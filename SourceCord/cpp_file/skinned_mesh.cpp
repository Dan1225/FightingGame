#include "../header_file/misc.h"
#include "../header_file/skinned_mesh.h"
#include "../header_file/shader.h"
#include "../header_file/texture.h"
#include <sstream>
#include <functional>
#include <filesystem>

using namespace DirectX;
inline XMFLOAT4X4 to_xmfloat4x4(const FbxAMatrix& fbxamatrix)
{
	XMFLOAT4X4 xmfloat4x4;
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < 4; column++)
		{
			xmfloat4x4.m[row][column] = static_cast<float>(fbxamatrix[row][column]);
		}
	}
	return xmfloat4x4;
}
inline XMFLOAT3 to_xmfloat3(const FbxDouble3& fbxdouble3)
{
	XMFLOAT3 xmfloat3;
	xmfloat3.x = static_cast<float>(fbxdouble3[0]);
	xmfloat3.y = static_cast<float>(fbxdouble3[1]);
	xmfloat3.z = static_cast<float>(fbxdouble3[2]);
	return xmfloat3;
}
inline XMFLOAT4 to_xmfloat4(const FbxDouble4& fbxdouble4)
{
	XMFLOAT4 xmfloat4;
	xmfloat4.x = static_cast<float>(fbxdouble4[0]);
	xmfloat4.y = static_cast<float>(fbxdouble4[1]);
	xmfloat4.z = static_cast<float>(fbxdouble4[2]);
	xmfloat4.w = static_cast<float>(fbxdouble4[3]);
	return xmfloat4;
}
skinned_mesh::skinned_mesh(ID3D11Device* device, const char *fbx_filename, bool triangulate)
{
	FbxManager* fbx_manager{ FbxManager::Create() };
	FbxScene * fbx_scene{ FbxScene::Create(fbx_manager, "") };

	FbxImporter * fbx_importer{ FbxImporter::Create(fbx_manager, "") };
	bool import_status{ false };
	import_status = fbx_importer->Initialize(fbx_filename);
	_ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

	import_status = fbx_importer->Import(fbx_scene);
	_ASSERT_EXPR_A(import_status, fbx_importer->GetStatus().GetErrorString());

	FbxGeometryConverter fbx_converter(fbx_manager);
	if (triangulate)
	{
		fbx_converter.Triangulate(fbx_scene, true/*replace*/, false/*legacy*/);
		fbx_converter.RemoveBadPolygonsFromMeshes(fbx_scene);
	}

	std::function<void(FbxNode*)> traverse{ [&](FbxNode* fbx_node) {
		scene::node& node{scene_view.nodes.emplace_back()};
		node.attribute = fbx_node->GetNodeAttribute() ?
			fbx_node->GetNodeAttribute()->GetAttributeType() : FbxNodeAttribute::EType::eUnknown;
		node.name = fbx_node->GetName();
		node.unique_id = fbx_node->GetUniqueID();
		node.parent_index = scene_view.indexof(fbx_node->GetParent() ?
		   fbx_node->GetParent()->GetUniqueID() : 0);
		for (int child_index = 0; child_index < fbx_node->GetChildCount(); ++child_index)
		{
			traverse(fbx_node->GetChild(child_index));
		}
	} };
	traverse(fbx_scene->GetRootNode());

	fetch_meshes(fbx_scene, meshes);
	//UNIT.19
	fetch_materials(fbx_scene, materials);
#if 1
	for (const scene::node& node : scene_view.nodes)
	{
		FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
		//Display node data in the output window as debug
		std::string node_name = fbx_node->GetName();
		uint64_t uid = fbx_node->GetUniqueID();
		uint64_t parent_uid = fbx_node->GetParent() ? fbx_node->GetParent()->GetUniqueID() : 0;
		int32_t type = fbx_node->GetNodeAttribute() ? fbx_node->GetNodeAttribute()->GetAttributeType() : 0;

		std::stringstream debug_string;
		debug_string << node_name << ":" << uid << ":" << parent_uid << ":" << type << "\n";
		OutputDebugStringA(debug_string.str().c_str());
	}
#endif
	fbx_manager->Destroy();

	create_com_objects(device, fbx_filename);
}

void skinned_mesh::fetch_meshes(FbxScene* fbx_scene, std::vector<mesh>& meshes)
{
	for (const scene::node& node : scene_view.nodes)
	{
		if (node.attribute != FbxNodeAttribute::EType::eMesh)
		{
			continue;
		}

		FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };
		FbxMesh* fbx_mesh{ fbx_node->GetMesh() };

		mesh& mesh{ meshes.emplace_back() };
		mesh.unique_id = fbx_mesh->GetNode()->GetUniqueID();
		mesh.name = fbx_mesh->GetNode()->GetName();
		mesh.node_index = scene_view.indexof(mesh.unique_id);

		//UNIT.20
		std::vector<mesh::subset>& subsets{ mesh.subsets };
		const int material_count{ fbx_mesh->GetNode()->GetMaterialCount() };
		subsets.resize(material_count > 0 ? material_count : 1);
		for (int material_index = 0; material_index < material_count; ++material_index)
		{
			const FbxSurfaceMaterial* fbx_material{ fbx_mesh->GetNode()->GetMaterial(material_index) };
			subsets.at(material_index).material_name = fbx_material->GetName();
			subsets.at(material_index).material_unique_id = fbx_material->GetUniqueID();
		}
		if (material_count > 0)
		{
			const int polygon_count{ fbx_mesh->GetPolygonCount() };
			for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
			{
				const int material_index{ fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) };
				subsets.at(material_index).index_count += 3;
			}
			uint32_t offset{ 0 };
			for (mesh::subset& subset : subsets)
			{
				subset.start_index_location = offset;
				offset += subset.index_count;
				subset.index_count = 0;
			}
		}
		//面の数を取得する
		const int polygon_count{ fbx_mesh->GetPolygonCount() };
		mesh.vertices.resize(polygon_count * 3LL);
		mesh.indices.resize(polygon_count * 3Ll);
		//UV座標の取得には、UVセット名が必要
		FbxStringList uv_names;
		//UVセット名のリストを取得する
		fbx_mesh->GetUVSetNames(uv_names);
		const FbxVector4* control_points{ fbx_mesh->GetControlPoints() };
		for (int polygon_index = 0; polygon_index < polygon_count; ++polygon_index)
		{
			const int material_index{ material_count > 0 ? fbx_mesh->GetElementMaterial()->GetIndexArray().GetAt(polygon_index) : 0 };
			mesh::subset& subset{ subsets.at(material_index) };
			const uint32_t offset{ subset.start_index_location + subset.index_count };

			for (int position_in_polygon = 0; position_in_polygon < 3; ++position_in_polygon)
			{
				const int vertex_index{ polygon_index * 3 + position_in_polygon };

				vertex vertex;
				//頂点インデックスを取得
				const int polygon_vertex{ fbx_mesh->GetPolygonVertex(polygon_index,position_in_polygon) };
				vertex.position.x = static_cast<float>(control_points[polygon_vertex][0]);
				vertex.position.y = static_cast<float>(control_points[polygon_vertex][1]);
				vertex.position.z = static_cast<float>(control_points[polygon_vertex][2]);

				if (fbx_mesh->GetElementNormalCount() > 0)
				{
					FbxVector4 normal;
					//法線が取得可能になる
					fbx_mesh->GetPolygonVertexNormal(polygon_index, position_in_polygon, normal);
					vertex.normal.x = static_cast<float>(normal[0]);
					vertex.normal.y = static_cast<float>(normal[1]);
					vertex.normal.z = static_cast<float>(normal[2]);
				}
				if (fbx_mesh->GetElementUVCount() > 0)
				{
					FbxVector2 uv;
					bool unmapped_uv;
					//UV座標を取得する
					fbx_mesh->GetPolygonVertexUV(polygon_index, position_in_polygon, uv_names[0], uv, unmapped_uv);
					vertex.texcoord.x = static_cast<float>(uv[0]);
					vertex.texcoord.y = 1.0f - static_cast<float>(uv[1]);
				}

				mesh.vertices.at(vertex_index) = std::move(vertex);
				mesh.indices.at(static_cast<size_t>(offset) + position_in_polygon) = vertex_index;
				subset.index_count++;
			}
		}
		/*21回目*/
		mesh.DefaultGlobalTransform = to_xmfloat4x4(fbx_mesh->GetNode()->EvaluateGlobalTransform());
	}
}

void skinned_mesh::create_com_objects(ID3D11Device* device, const char* fbx_filename)
{
	for (mesh& mesh : meshes)
	{
		HRESULT hr{ S_OK };
		D3D11_BUFFER_DESC buffer_desc{};
		D3D11_SUBRESOURCE_DATA subresource_data{};
		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * mesh.vertices.size());
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = mesh.vertices.data();
		subresource_data.SysMemPitch = 0;
		subresource_data.SysMemSlicePitch = 0;
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.vertex_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * mesh.indices.size());
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		subresource_data.pSysMem = mesh.indices.data();
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, mesh.index_buffer.ReleaseAndGetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
#if 1
		mesh.vertices.clear();
		mesh.indices.clear();
#endif
	}

	HRESULT hr = S_OK;
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shaders/skinned_mesh_vs.cso", vertex_shader.ReleaseAndGetAddressOf(), input_layout.ReleaseAndGetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	create_ps_from_cso(device, "Shaders/skinned_mesh_ps.cso", pixel_shader.ReleaseAndGetAddressOf());

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(constants);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//UNIT.19
	for (std::unordered_map<uint64_t, material>::iterator iterator = materials.begin();iterator != materials.end(); ++iterator)
	{
		if (iterator->second.texture_filenames[0].size() > 0)
		{
			std::filesystem::path path(fbx_filename);
			path.replace_filename(iterator->second.texture_filenames[0]);
			D3D11_TEXTURE2D_DESC texture2d_desc;
			load_texture_from_file(device, path.c_str(),
			iterator->second.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
		}
		else
		{
			make_dummy_texture(device, iterator->second.shader_resource_views[0].GetAddressOf(),
			0xFFFFFFFF, 16);
		}
	}
}

void skinned_mesh::render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color)
{
	for (const mesh& mesh : meshes)
	{
		uint32_t stride{ sizeof(vertex) };
		uint32_t offset{ 0 };
		device_context->IASetVertexBuffers(0, 1, mesh.vertex_buffer.GetAddressOf(), &stride, &offset);
		device_context->IASetIndexBuffer(mesh.index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		device_context->IASetInputLayout(input_layout.Get());

		device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
		device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

		//UNIT.19
		//for (std::unordered_map<uint64_t, material>::iterator iterator = materials.begin(); iterator != materials.end(); ++iterator)
		//{
			//UNIT.19
			//device_context->PSSetShaderResources(0, 1, materials.cbegin()->second.shader_resource_views[0].GetAddressOf());
		constants data;
		data.world = world;
		data.material_color = material_color;
		//UNIT.21
		XMStoreFloat4x4(&data.world,XMLoadFloat4x4(&mesh.DefaultGlobalTransform) * XMLoadFloat4x4(&world));
#if 0
		for (const mesh::subset& subset : mesh.subsets)
		{
			//UNIT.20
			const material& material{ materials.at(subset.material_unique_id) };
			XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));

			device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
			device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			//UNIT.19
			//XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&materials.cbegin()->second.Kd));


			device_context->PSSetShaderResources(0, 1, material.shader_resource_views[0].GetAddressOf());

			//D3D11_BUFFER_DESC buffer_desc;
			//mesh.index_buffer->GetDesc(&buffer_desc);
			device_context->DrawIndexed(subset.index_count, subset.start_index_location, 0);
		}
#else
		for (const mesh::subset& subset : mesh.subsets)
		{
			if (subset.material_unique_id != 0)
			{
				//UNIT.20
				const material& material{ materials.at(subset.material_unique_id) };
				if (materials.size() > 0)
				{
					//XMStoreFloat4x4(&data.world,
					//	XMLoadFloat4x4(&mesh.DefaultGlobalTransform) * XMLoadFloat4x4(&world));
					//XMStoreFloat4(&data.material_color,XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));
					device_context->PSSetShaderResources(0, 1,material.shader_resource_views[0].GetAddressOf());
				}

			}
			else
			{
				//device_context->PSSetShaderResources(0,1,)
				//ここでダミーを設定したい
				device_context->PSSetShaderResources(0, 1,
					Dummy.GetAddressOf());
			}
			device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
			device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());
			//UNIT.19
			//XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&materials.cbegin()->second.Kd));
			D3D11_BUFFER_DESC buffer_desc;
			mesh.index_buffer->GetDesc(&buffer_desc);
			//D3D11_BUFFER_DESC buffer_desc;
			//mesh.index_buffer->GetDesc(&buffer_desc);
			device_context->DrawIndexed(subset.index_count, subset.start_index_location, 0);
		}

#endif // 0
		//}
	}
}

//UNIT.19
void skinned_mesh::fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials)
{
	const size_t node_count{ scene_view.nodes.size() };
	for (size_t node_index = 0; node_index < node_count; ++node_index)
	{
		const scene::node& node{ scene_view.nodes.at(node_index) };
		const FbxNode* fbx_node{ fbx_scene->FindNodeByName(node.name.c_str()) };

		const int material_count{ fbx_node->GetMaterialCount() };
		for (int material_index = 0; material_index < material_count; ++material_index)
		{
			const FbxSurfaceMaterial* fbx_material{ fbx_scene->GetMaterial(material_index) };

			material material;
			material.name = fbx_material->GetName();
			material.unique_id = fbx_material->GetUniqueID();
			FbxProperty fbx_property;
			fbx_property = fbx_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
			if (fbx_property.IsValid())
			{
				const FbxDouble3 color{ fbx_property.Get<FbxDouble3>() };
				material.Ka.x = static_cast<float>(color[0]);
				material.Ka.y = static_cast<float>(color[1]);
				material.Ka.z = static_cast<float>(color[2]);
				material.Ka.w = 1.0f;
				material.Kd.x = static_cast<float>(color[0]);
				material.Kd.y = static_cast<float>(color[1]);
				material.Kd.z = static_cast<float>(color[2]);
				material.Kd.w = 1.0f;
				material.Ks.x = static_cast<float>(color[0]);
				material.Ks.y = static_cast<float>(color[1]);
				material.Ks.z = static_cast<float>(color[2]);
				material.Ks.w = 1.0f;

				const FbxFileTexture* fbx_texture{ fbx_property.GetSrcObject<FbxFileTexture>() };
				material.texture_filenames[0] = fbx_texture ? fbx_texture->GetRelativeFileName() : "";
			}
			materials.emplace(material.unique_id, std::move(material));
		}
	}
}