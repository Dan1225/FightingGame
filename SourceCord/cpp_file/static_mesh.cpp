// UNIT.13
#include "../header_file/shader.h"
#include "../header_file/misc.h"
#include "../header_file/static_mesh.h"

#include <fstream>
#include <vector>

// UNIT.14
#include <filesystem>
#include "../header_file/texture.h"

// UNIT.13
using namespace DirectX;
static_mesh::static_mesh(
	ID3D11Device* device,
	const wchar_t* obj_filename,
	bool flipping_v_coordinates/*UNIT.14*/)
{
	std::vector<vertex> vertices;
	std::vector<uint32_t> indices;
	uint32_t current_index{ 0 };

	std::vector<XMFLOAT3> positions;
	std::vector<XMFLOAT3> normals;
	// UNIT.14
	std::vector<XMFLOAT2> texcoords;
	std::vector<std::wstring> mtl_filenames;

	std::wifstream fin(obj_filename);
	_ASSERT_EXPR(fin, L"'OBJ file not found.");
	wchar_t command[256];
	while (fin)
	{
		fin >> command;
		if (0 == wcscmp(command, L"v"))
		{
			// V X Y Z W
			//
			// 幾何学的な頂点とそのx y z座標を指定する．
			// 有理曲線や曲面には，4番目の同次座標が必要である．
			// 重みと呼ばれる．
			//
			// x y z は，頂点の x, y, z 座標である．これらは,
			// 3次元における頂点の位置を定義する浮動小数点数です．これらは浮動小数点数であり，
			// // 3次元における頂点の位置を定義する．
			//
			// w は，有理曲線および曲面に必要な重みで，非有理曲線および曲面には
			// // 必要ありません．非有理曲線とサーフェスには必要ありません。
			// wに値を指定しない場合、デフォルトは1.0です
			float x, y, z;
			fin >> x >> y >> z;
			positions.push_back({ x, y, z });
			fin.ignore(1024, L'\n');
		}
		// UNIT.14
		else if (0 == wcscmp(command, L"vt"))
		{
			// vt u v w
			//
			// テクスチャの頂点とその座標を指定します．1D テクスチャは u テクスチャ座標のみを必要とし，
			// 2D テクスチャは u と v テクスチャ座標の両方を必要とします．
			// と v のテクスチャ座標が必要です。
			// 座標を指定します。
			//
			// u はテクスチャの水平方向を表す値です。
			// vはオプションの引数です。
			// v はテクスチャの垂直方向の値です。
			// デフォルトは 0 です。
			// w はオプションの引数です。
			// wはテクスチャの深さを表す値です。デフォルトは0です。
			float u, v;
			fin >> u >> v;
			//texcoords.push_back({ u, v });
			texcoords.push_back({ u, flipping_v_coordinates ? 1.0f - v : v });
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"vn"))
		{
			// vn i j k
		//
		// 成分i、j、kの法線ベクトルを指定します。
		//
		// 頂点法線は，ジオメトリのスムーズなシェーディングとレンダリングに影響を与えます。
		// ポリゴンの場合，頂点法線は実際のファセット法線の代わりに使用される。
		// ポリゴンの場合、頂点法線は実際のファセット法線の代わりに使用されます。 // サーフェスの場合、頂点法線はサーフェス全体に渡って補間され
		// サーフェスの場合、頂点法線はサーフェス全体にわたって補間され、実際の解析的なサーフェス法線に置き換えられます。
		//
		// 頂点法線が存在する場合は、スムージンググループよりも優先されます。
		//
		// i j k は，頂点法線の i, j, k 座標です．
		// は浮動小数点数です。
			FLOAT i, j, k;
			fin >> i >> j >> k;
			normals.push_back({ i, j, k });
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"f"))
		{
			//f v1 / vt1 / vn1 v2 / vt2 / vn2 v3 / vt3 / vn3 ... ...
			//
			// オプションとして、テクスチャの頂点と頂点の法線の参照番号を含めることができます。
			// 数値
			// 頂点、テクスチャ頂点、および頂点法線の参照番号は、 // 必要です。頂点、テクスチャ頂点、頂点法線の参照番号は
			// // スラッシュ（/）で区切らなければなりません。
			// 番号とスラッシュの間にはスペースはありません。
			// v は面の要素の頂点の参照番号です。
			// 最低3つの頂点が必要です．
			// vt はオプションの引数です．
			// vt は face 要素内のテクスチャ頂点の参照番号です。
			// これは常に最初のスラッシュの後に続きます。
			// vn はオプションの引数です。
			// vnはface要素内の頂点法線の参照番号です。
			// これは常に2番目のスラッシュの後に続く必要があります．
			// 面の要素は面の法線を使ってその方向を示します。
			// 頂点が面を中心に反時計回りに並んでいる場合、面と法線の両方が
			// 頂点が顔の周りで反時計回りに並んでいる場合、顔と法線の両方がビューアの方を向きます。
			// 頂点の順序が時計回りの場合、両方ともビューアーから離れたところを指します。
			//頂点法線が割り当てられている場合、それらは表面法線の一般的な方向を向いていなければなりません。
			// 予測できない結果が生じる可能性があります。
			// 面にテクスチャマップが割り当てられていて、f文でテクスチャ頂点が割り当てられていない場合は
			// 面にテクスチャマップが割り当てられていて、fステートメントでテクスチャ頂点が割り当てられていない場合、要素のレンダリング時にテクスチャマップは無視されます。
			// 要素がレンダリングされるとき、テクスチャマップは無視されます。
			for (size_t i = 0; i < 3; i++)
			{
				vertex vertex;
				size_t v, vt, vn;

				fin >> v;
				vertex.position = positions.at(v - 1);
				if (L'/' == fin.peek())
				{
					fin.ignore(1);
					if (L'/' != fin.peek())
					{
						fin >> vt;
						// UNIT.14
						vertex.texcoord = texcoords.at(vt - 1);
					}
					if (L'/' == fin.peek())
					{
						fin.ignore(1);
						fin >> vn;
						vertex.normal = normals.at(vn - 1);
					}
				}
				vertices.push_back(vertex);
				indices.push_back(current_index++);
			}
			fin.ignore(1024, L'\n');
		}
		// UNIT.14
		else if (0 == wcscmp(command, L"mtllib"))
		{
			// mtllib ファイル名1 ファイル名2 . . .
			// usemtlステートメントで設定された材料定義の材料ライブラリファイルを指定します。
			// mtllibでは複数のファイル名を指定することができます。
			// 複数のファイル名が指定された場合は、最初にリストアップされたファイルから材料定義が検索されます。
			// 複数のファイル名が指定された場合、最初に材料定義が検索され、次に2番目のファ
			// ファイル名を複数指定することができます。
			wchar_t mtllib[256];
			fin >> mtllib;
			mtl_filenames.push_back(mtllib);
		}
		// UNIT.15
		else if (0 == wcscmp(command, L"usemtl"))
		{
			wchar_t usemtl[MAX_PATH] = { 0 };
			fin >> usemtl;
			subsets.push_back({ usemtl,static_cast<uint32_t>(indices.size()),0 });
		}
		else
		{
			fin.ignore(1024, L'\n');
		}
	}
	fin.close();
	//UNIT.16
	if (materials.size() == 0)
	{
		for (const subset& subset : subsets)
		{
			materials.push_back({ subset.usemtl });
		}
	}

	// UNIT.15
	std::vector<subset>::reverse_iterator iterator = subsets.rbegin();
	iterator->index_count = indices.size() - iterator->index_start;
	for (iterator = subsets.rbegin() + 1; iterator != subsets.rend(); ++iterator)
	{
		iterator->index_count = (iterator - 1)->index_start - iterator->index_start;
	}

	//UNIT14
	std::filesystem::path mtl_filename(obj_filename);
	mtl_filename.replace_filename(std::filesystem::path(mtl_filenames[0]).filename());

	fin.open(mtl_filename);
	//_ASSERT_EXPR(fin, L"MTL file not found");

	while (fin)
	{
		fin >> command;
		if (0 == wcscmp(command, L"map_Kd"))
		{
			fin.ignore();
			wchar_t map_Kd[256];
			fin >> map_Kd;

			std::filesystem::path path(obj_filename);
			path.replace_filename(std::filesystem::path(map_Kd).filename());
			//texture_filename = path;
			//UNIT.15
			//materials.rbegin()->texture_filename = path;
			//UNIT.16
			materials.rbegin()->texture_filenames[0] = path;
			fin.ignore(1024, L'\n');
		}
		//UNIT.15
		//UNIT.16
		else if (0 == wcscmp(command, L"map_bump") || 0 == wcscmp(command, L"bump"))
		{
			fin.ignore();
			//UNIT.16
			wchar_t map_bump[256];
			fin >> map_bump;
			std::filesystem::path path(obj_filename);
			path.replace_filename(std::filesystem::path(map_bump).filename());
			materials.rbegin()->texture_filenames[1] = path;
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"Ka"))
		{
			float r, g, b;
			fin >> r >> g >> b;
			materials.rbegin()->Ka = { r,g,b,1 };
			fin.ignore(1024, L'\n');
		}
		//UNIT.15
		else if (0 == wcscmp(command, L"Kd"))
		{
			float r, g, b;
			fin >> r >> g >> b;
			materials.rbegin()->Kd = { r,g,b,1 };
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"Ks"))
		{
			float r, g, b;
			fin >> r >> g >> b;
			materials.rbegin()->Ks = { r,g,b,1 };
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"newmtl"))
		{
			/*newmtlまで読んだら、そのあとにそれを適用してるマテリアルの名前がある
			それをNewMtlに格納して、ヘッダーで作った配列の中のNameに保存*/
			fin.ignore();
			wchar_t NewMtl[256];
			material MTL;
			fin >> NewMtl;
			MTL.name = NewMtl;
			materials.push_back(MTL);

		}
		else
		{
			fin.ignore(1024, L'\n');
		}
	}
	fin.close();

	create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());

	HRESULT hr{ S_OK };
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		// UNIT.14
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shaders/static_mesh_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, ARRAYSIZE(input_element_desc));
	create_ps_from_cso(device, "Shaders/static_mesh_ps.cso", pixel_shader.GetAddressOf());

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(constants);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//UNIT.14
	D3D11_TEXTURE2D_DESC texture2d_desc{};
	//load_texture_from_file(device, texture_filename.c_str(),
		//shader_resource_view.GetAddressOf(),&texture2d_desc);
	//UNIT.15
	//テクスチャのロード
	for (material& material : materials)
	{
		//UNIT.16
		if (material.texture_filenames[0].size() >= 1)
		{
			load_texture_from_file(device, material.texture_filenames[0].c_str(),
				material.shader_resource_views[0].GetAddressOf(), &texture2d_desc);
		}
		else
		{
			make_dummy_texture(device, material.shader_resource_views[0].GetAddressOf(), 0xFFFFFFFF, 16);
		}
		if (material.texture_filenames[1].size() >= 1)
		{
			load_texture_from_file(device, material.texture_filenames[1].c_str(),
				material.shader_resource_views[1].GetAddressOf(), &texture2d_desc);
		}
		else
		{
			make_dummy_texture(device, material.shader_resource_views[1].GetAddressOf(), 0xFFFFFFFF, 16);
		}
	}
}

// UNIT.13
void static_mesh::render(ID3D11DeviceContext* device_context, const XMFLOAT4X4& world, const XMFLOAT4& material_color)
{
	uint32_t stride{ sizeof(vertex) };
	uint32_t offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->IASetInputLayout(input_layout.Get());

	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	/*D3D11_BUFFER_DESC buffer_desc{};
	index_buffer->GetDesc(&buffer_desc);
	device_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);*/
	for (const material& material : materials)
	{
		// UNIT.14
		// device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());//テクスチャが反映された
		// UNIT.15
		device_context->PSSetShaderResources(0, 1, material.shader_resource_views[0].GetAddressOf());//テクスチャが反映された
		device_context->PSSetShaderResources(1, 1, material.shader_resource_views[1].GetAddressOf());//テクスチャが反映された


		constants data{ world, material_color };
		//UNIT.15
		XMStoreFloat4(&data.material_color, XMLoadFloat4(&material_color) * XMLoadFloat4(&material.Kd));
		device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
		device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

		for (const subset& subset : subsets)
		{
			if (material.name == subset.usemtl)
			{
				device_context->DrawIndexed(subset.index_count, subset.index_start, 0);
			}
		}
	}
}

// UNIT.13
void static_mesh::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count, uint32_t* indices, size_t index_count)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC buffer_desc{};
	D3D11_SUBRESOURCE_DATA subresource_data{};
	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(vertex) * vertex_count);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	subresource_data.pSysMem = vertices;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	buffer_desc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * index_count);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	subresource_data.pSysMem = indices;
	hr = device->CreateBuffer(&buffer_desc, &subresource_data, index_buffer.ReleaseAndGetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}