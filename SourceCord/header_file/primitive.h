#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
using namespace DirectX;
class Primitive
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>       vertex_buffer;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_state;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencil_State;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;
public:
	Primitive(ID3D11Device* device);
	~Primitive();
	void rect(ID3D11DeviceContext* device_context,
		const XMFLOAT2& pos, const XMFLOAT2& size,
		const XMFLOAT2& center,
		float angle,
		const XMFLOAT4 color) const;
	void draw(ID3D11DeviceContext* device_context,
		float x, float y, float width, float height,
		float cx, float cy, float angle,
		float r, float g, float b, float a) const;
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
};