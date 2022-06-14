#include "../header_file/geometric_primitive.h"
#include "../header_file/shader.h"
#include "../header_file/misc.h"
geometric_primitive::geometric_primitive(ID3D11Device* device)
{
	HRESULT hr{ S_OK };

	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(device, "Shaders/geometric_primitive_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shaders/geometric_primitive_ps.cso", pixel_shader.GetAddressOf());

	//UNIT.12
	D3D11_RASTERIZER_DESC rasterizer_desc{};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;   //ソリッド
	rasterizer_desc.CullMode = D3D11_CULL_BACK;    //裏面ポリゴンをカリングします(カリングはいらない要素を摘み取る意味)
	rasterizer_desc.FrontCounterClockwise = FALSE; // 反時計回り(counter clockwise)の頂点を持つ面を表と見なす
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = TRUE;
	rasterizer_desc.ScissorEnable = FALSE;
	rasterizer_desc.MultisampleEnable = FALSE;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.AntialiasedLineEnable = FALSE;
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	rasterizer_desc.AntialiasedLineEnable = TRUE;
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_states[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(constants);
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
}

void geometric_primitive::render(ID3D11DeviceContext* device_context,
	const XMFLOAT4X4& world, const XMFLOAT4& material_color, bool wireframe)
{
	uint32_t stride{ sizeof(vertex) };
	uint32_t offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetIndexBuffer(index_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	device_context->IASetInputLayout(input_layout.Get());

	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	constants data{ world, material_color };
	device_context->UpdateSubresource(constant_buffer.Get(), 0, 0, &data, 0, 0);
	device_context->VSSetConstantBuffers(0, 1, constant_buffer.GetAddressOf());

	if (wireframe)
	{
		device_context->RSSetState(rasterizer_states[1].Get());
	}
	else
	{
		device_context->RSSetState(rasterizer_states[0].Get());
	}

	D3D11_BUFFER_DESC buffer_desc{};
	index_buffer->GetDesc(&buffer_desc);
	device_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(uint32_t), 0, 0);
}

void geometric_primitive::create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
	uint32_t* indices, size_t index_count)
{
	HRESULT hr{ S_OK };

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

//UNIT.12
geometric_cube::geometric_cube(ID3D11Device* device) : geometric_primitive(device)
{
	vertex vertices[24]{};
	//サイズが1.0の正立方体データを作成する(重心を原点にする)。正立方体のコントロールポイント数は8個、
	//1つのコントロールポイントの位置には法線の向きが違う頂点が3個あるので頂点情報の総数は8×3=24個、
	//頂点情報配列(vertices)にすべて頂点の位置、法線情報を格納する。
	uint32_t indices[36]{};
	//正立方体は6面持ち、1つの面は2つの3角形ポリゴンで構成されるので3角形ポリゴンの総数は6×2=12個、
	//正立方体を描画するために12回の3角形ポリゴン描画が必要、よって参照される頂点情報は12×3=36回、
	//3角形ポリゴンが参照する頂点情報のインデックス(頂点番号)を描画順に配列(indices)に格納する。
	//時計回りが表面になるように格納すること。
	uint32_t face{ 0 };

	// top-side
	// 0---------1
	// |         |
	// |   -Y    |
	// |         |
	// 2---------3
	face = 0;
	vertices[face * 4 + 0].position = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2].position = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 3].position = { +0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 0].normal = { +0.0f, +1.0f, +0.0f };
	vertices[face * 4 + 1].normal = { +0.0f, +1.0f, +0.0f };
	vertices[face * 4 + 2].normal = { +0.0f, +1.0f, +0.0f };
	vertices[face * 4 + 3].normal = { +0.0f, +1.0f, +0.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;
	// bottom-side
	// 0---------1
	// |         |
	// |   -Y    |
	// |         |
	// 2---------3
	face += 1;
	vertices[face * 4 + 0].position = { -0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 1].position = { +0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3].position = { +0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 0].normal = { +0.0f, -1.0f, +0.0f };
	vertices[face * 4 + 1].normal = { +0.0f, -1.0f, +0.0f };
	vertices[face * 4 + 2].normal = { +0.0f, -1.0f, +0.0f };
	vertices[face * 4 + 3].normal = { +0.0f, -1.0f, +0.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	// front-side
	// 0---------1
	// |         |
	// |   +Z    |
	// |         |
	// 2---------3
	face += 1;
	vertices[face * 4 + 0].position = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1].position = { +0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3].position = { +0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 0].normal = { +0.0f, +0.0f, -1.0f };
	vertices[face * 4 + 1].normal = { +0.0f, +0.0f, -1.0f };
	vertices[face * 4 + 2].normal = { +0.0f, +0.0f, -1.0f };
	vertices[face * 4 + 3].normal = { +0.0f, +0.0f, -1.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;

	// back-side
	// 0---------1
	// |         |
	// |   +Z    |
	// |         |
	// 2---------3
	face += 1;
	vertices[face * 4 + 0].position = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2].position = { -0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 3].position = { +0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 0].normal = { +0.0f, +0.0f, +1.0f };
	vertices[face * 4 + 1].normal = { +0.0f, +0.0f, +1.0f };
	vertices[face * 4 + 2].normal = { +0.0f, +0.0f, +1.0f };
	vertices[face * 4 + 3].normal = { +0.0f, +0.0f, +1.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	// right-side
	// 0---------1
	// |         |
	// |   -X    |
	// |         |
	// 2---------3
	face += 1;
	vertices[face * 4 + 0].position = { +0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1].position = { +0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2].position = { +0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3].position = { +0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 0].normal = { +1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 1].normal = { +1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 2].normal = { +1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 3].normal = { +1.0f, +0.0f, +0.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 1;
	indices[face * 6 + 2] = face * 4 + 2;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 3;
	indices[face * 6 + 5] = face * 4 + 2;

	// left-side
	// 0---------1
	// |         |
	// |   -X    |
	// |         |
	// 2---------3
	face += 1;
	vertices[face * 4 + 0].position = { -0.5f, +0.5f, -0.5f };
	vertices[face * 4 + 1].position = { -0.5f, +0.5f, +0.5f };
	vertices[face * 4 + 2].position = { -0.5f, -0.5f, -0.5f };
	vertices[face * 4 + 3].position = { -0.5f, -0.5f, +0.5f };
	vertices[face * 4 + 0].normal = { -1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 1].normal = { -1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 2].normal = { -1.0f, +0.0f, +0.0f };
	vertices[face * 4 + 3].normal = { -1.0f, +0.0f, +0.0f };
	indices[face * 6 + 0] = face * 4 + 0;
	indices[face * 6 + 1] = face * 4 + 2;
	indices[face * 6 + 2] = face * 4 + 1;
	indices[face * 6 + 3] = face * 4 + 1;
	indices[face * 6 + 4] = face * 4 + 2;
	indices[face * 6 + 5] = face * 4 + 3;

	create_com_buffers(device, vertices, 24, indices, 36);
}

#include <vector>
geometric_cylinder::geometric_cylinder(ID3D11Device* device, u_int slices) : geometric_primitive(device)
{
	std::vector<vertex> vertices;
	std::vector<u_int> indices;

	float d = 2.0f * DirectX::XM_PI / slices;
	float r = 0.5f;

	vertex vertex;
	u_int base_index = 0;

	// top cap centre
	vertex.position = DirectX::XMFLOAT3(0.0f, +0.5f, 0.0f);
	vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);
	vertices.push_back(vertex);
	// top cap ring
	for (u_int i = 0; i < slices; ++i)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);
		vertex.position = DirectX::XMFLOAT3(x, +0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);
		vertices.push_back(vertex);
	}
	base_index = 0;
	for (u_int i = 0; i < slices - 1; ++i)
	{
		indices.push_back(base_index + 0);
		indices.push_back(base_index + i + 2);
		indices.push_back(base_index + i + 1);
	}
	indices.push_back(base_index + 0);
	indices.push_back(base_index + 1);
	indices.push_back(base_index + slices);

	// bottom cap centre
	vertex.position = DirectX::XMFLOAT3(0.0f, -0.5f, 0.0f);
	vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	vertices.push_back(vertex);
	// bottom cap ring
	for (u_int i = 0; i < slices; ++i)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);
		vertex.position = DirectX::XMFLOAT3(x, -0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
		vertices.push_back(vertex);
	}
	base_index = slices + 1;
	for (u_int i = 0; i < slices - 1; ++i)
	{
		indices.push_back(base_index + 0);
		indices.push_back(base_index + i + 1);
		indices.push_back(base_index + i + 2);
	}
	indices.push_back(base_index + 0);
	indices.push_back(base_index + (slices - 1) + 1);
	indices.push_back(base_index + (0) + 1);

	// side rectangle
	for (u_int i = 0; i < slices; ++i)
	{
		float x = r * cosf(i * d);
		float z = r * sinf(i * d);

		vertex.position = DirectX::XMFLOAT3(x, +0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(x, 0.0f, z);
		vertices.push_back(vertex);

		vertex.position = DirectX::XMFLOAT3(x, -0.5f, z);
		vertex.normal = DirectX::XMFLOAT3(x, 0.0f, z);
		vertices.push_back(vertex);
	}
	base_index = slices * 2 + 2;
	for (u_int i = 0; i < slices - 1; ++i)
	{
		indices.push_back(base_index + i * 2 + 0);
		indices.push_back(base_index + i * 2 + 2);
		indices.push_back(base_index + i * 2 + 1);

		indices.push_back(base_index + i * 2 + 1);
		indices.push_back(base_index + i * 2 + 2);
		indices.push_back(base_index + i * 2 + 3);
	}
	indices.push_back(base_index + (slices - 1) * 2 + 0);
	indices.push_back(base_index + (0) * 2 + 0);
	indices.push_back(base_index + (slices - 1) * 2 + 1);

	indices.push_back(base_index + (slices - 1) * 2 + 1);
	indices.push_back(base_index + (0) * 2 + 0);
	indices.push_back(base_index + (0) * 2 + 1);

	create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());
}

geometric_sphere::geometric_sphere(ID3D11Device* device, u_int slices, u_int stacks) : geometric_primitive(device)
{
	std::vector<vertex> vertices;
	std::vector<u_int> indices;

	float r = 0.5f;

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	vertex top_vertex;
	top_vertex.position = DirectX::XMFLOAT3(0.0f, +r, 0.0f);
	top_vertex.normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);

	vertex bottom_vertex;
	bottom_vertex.position = DirectX::XMFLOAT3(0.0f, -r, 0.0f);
	bottom_vertex.normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices.push_back(top_vertex);

	float phi_step = DirectX::XM_PI / stacks;
	float theta_step = 2.0f * DirectX::XM_PI / slices;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (u_int i = 1; i <= stacks - 1; ++i)
	{
		float phi = i * phi_step;

		// Vertices of ring.
		for (u_int j = 0; j <= slices; ++j)
		{
			float theta = j * theta_step;

			vertex v;

			// spherical to cartesian
			v.position.x = r * sinf(phi) * cosf(theta);
			v.position.y = r * cosf(phi);
			v.position.z = r * sinf(phi) * sinf(theta);

			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&v.position);
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

			vertices.push_back(v);
		}
	}

	vertices.push_back(bottom_vertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//
	for (UINT i = 1; i <= slices; ++i)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	u_int base_index = 1;
	u_int ring_vertex_count = slices + 1;
	for (u_int i = 0; i < stacks - 2; ++i)
	{
		for (u_int j = 0; j < slices; ++j)
		{
			indices.push_back(base_index + i * ring_vertex_count + j);
			indices.push_back(base_index + i * ring_vertex_count + j + 1);
			indices.push_back(base_index + (i + 1) * ring_vertex_count + j);

			indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
			indices.push_back(base_index + i * ring_vertex_count + j + 1);
			indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	u_int south_pole_index = (u_int)vertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	base_index = south_pole_index - ring_vertex_count;

	for (u_int i = 0; i < slices; ++i)
	{
		indices.push_back(south_pole_index);
		indices.push_back(base_index + i);
		indices.push_back(base_index + i + 1);
	}
	create_com_buffers(device, vertices.data(), vertices.size(), indices.data(), indices.size());
}