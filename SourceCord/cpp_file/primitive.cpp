#include "../header_file/primitive.h"
#include "../header_file/misc.h"
#include "../Graphics/framework.h"
#include <sstream>

Primitive::Primitive(ID3D11Device* device) :
	vertex_shader(nullptr), pixel_shader(nullptr), input_layout(nullptr), vertex_buffer(nullptr), rasterizer_state(nullptr)
{
	HRESULT hr{ S_OK };

	vertex vertices[] = { XMFLOAT3(0, 0, 0), XMFLOAT4(0, 0, 0, 0) };

	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertices);
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = vertices;
	subresource_data.SysMemPitch = 0;
	subresource_data.SysMemSlicePitch = 0;
	hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertex_buffer);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace);

	//頂点シェーダーオブジェクトの生成
	const char* cso_name{ "Shaders/primitive_vs.cso" };

	FILE* fp{};
	fopen_s(&fp, cso_name, "rb");
	_ASSERT_EXPR_A(fp, "CSO File  not found");

	fseek(fp, 0, SEEK_END);
	long cso_sz{ ftell(fp) };
	fseek(fp, 0, SEEK_SET);

	std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);

	hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//入力レイアウトオブジェクトの生成
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
		  D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,
		  D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
	};
	hr = device->CreateInputLayout(input_element_desc, _countof(input_element_desc),
		cso_data.get(), cso_sz, input_layout.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//ピクセルシェーダーオブジェクトの生成
	{
		const char* cso_name{ "Shaders/primitive_ps.cso" };

		FILE* fp{};
		fopen_s(&fp, cso_name, "rb");
		_ASSERT_EXPR_A(fp, "CSO File  not found");

		fseek(fp, 0, SEEK_END);
		long cso_sz{ ftell(fp) };
		fseek(fp, 0, SEEK_SET);

		std::unique_ptr<unsigned char[]> cso_data{ std::make_unique<unsigned char[]>(cso_sz) };
		fread(cso_data.get(), cso_sz, 1, fp);
		fclose(fp);

		hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	//ラスタライザ―設定
	D3D11_RASTERIZER_DESC rasterizer_desc{};
	rasterizer_desc.FillMode = D3D11_FILL_SOLID;   //ソリッド
	rasterizer_desc.CullMode = D3D11_CULL_NONE;	   //裏面ポリゴンをカリングします(カリングはいらない要素を摘み取る意味)
	rasterizer_desc.FrontCounterClockwise = false; // 反時計回り(counter clockwise)の頂点を持つ面を表と見なす
	rasterizer_desc.DepthBias = 0;
	rasterizer_desc.DepthBiasClamp = 0;
	rasterizer_desc.SlopeScaledDepthBias = 0;
	rasterizer_desc.DepthClipEnable = false;
	rasterizer_desc.ScissorEnable = false;
	rasterizer_desc.MultisampleEnable = false;
	rasterizer_desc.AntialiasedLineEnable = false;
	hr = device->CreateRasterizerState(&rasterizer_desc, rasterizer_state.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
	depth_stencil_desc.DepthEnable = FALSE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.StencilEnable = FALSE;
	depth_stencil_desc.StencilReadMask = 0xFF;
	depth_stencil_desc.StencilWriteMask = 0xFF;
	depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	hr = device->CreateDepthStencilState(&depth_stencil_desc, depthStencil_State.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

}

Primitive::~Primitive()
{

}

void Primitive::rect(
	ID3D11DeviceContext* device_context,
	const XMFLOAT2& pos, const XMFLOAT2& size,
	const XMFLOAT2& center,
	float angle,
	const XMFLOAT4 color) const
{
	if (size.x * size.y == 0.0f) return;

	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	device_context->RSGetViewports(&numViewports, &viewport);

	vertex vertices[] = {
		{ XMFLOAT3(-0.0f, +1.0f, 0), color },
		{ XMFLOAT3(+1.0f, +1.0f, 0), color },
		{ XMFLOAT3(-0.0f, -0.0f, 0), color },
		{ XMFLOAT3(+1.0f, -0.0f, 0), color },
	};

	float sinValue = sinf(angle);
	float cosValue = cosf(angle);
	for (int i = 0; i < 4; i++)
	{
		vertices[i].position.x *= size.x;
		vertices[i].position.y *= size.y;
		vertices[i].position.x -= center.x;
		vertices[i].position.y -= center.y;

		float rx = vertices[i].position.x;
		float ry = vertices[i].position.y;
		vertices[i].position.x = rx * cosValue - ry * sinValue;
		vertices[i].position.y = rx * sinValue + ry * cosValue;
		vertices[i].position.x += pos.x;
		vertices[i].position.y += pos.y;

		vertices[i].position.x = vertices[i].position.x * 2 / viewport.Width - 1.0f;
		vertices[i].position.y = 1.0f - vertices[i].position.y * 2 / viewport.Height;
	}

	D3D11_MAPPED_SUBRESOURCE msr;
	device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
	memcpy(msr.pData, vertices, sizeof(vertices));
	device_context->Unmap(vertex_buffer.Get(), 0);

	UINT stride = sizeof(vertex);
	UINT offset = 0;
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	device_context->IASetInputLayout(input_layout.Get());
	device_context->RSSetState(rasterizer_state.Get());
	device_context->VSSetShader(vertex_shader.Get(), NULL, 0);
	device_context->PSSetShader(pixel_shader.Get(), NULL, 0);
	device_context->OMSetDepthStencilState(depthStencil_State.Get(), 1);

	device_context->Draw(4, 0);
}

void Primitive::draw(
	ID3D11DeviceContext* device_context,
	float x, float y, float width, float height,
	float cx, float cy,
	float angle,
	float r, float g, float b, float a) const
{
	rect(device_context, XMFLOAT2(x, y), XMFLOAT2(width, height), XMFLOAT2(cx, cy), angle, XMFLOAT4(r, g, b, a));
}