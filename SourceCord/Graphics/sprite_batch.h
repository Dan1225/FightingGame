#pragma once
#include <d3d11.h>
#include <wrl.h>
using namespace Microsoft::WRL;
#include <DirectXMath.h>
using namespace DirectX;
#include <vector>

class sprite_batch
{
private:
	ComPtr<ID3D11VertexShader> vertex_shader;
	ComPtr<ID3D11PixelShader> pixel_shader;
	ComPtr<ID3D11InputLayout> input_layout;
	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

public:
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
		XMFLOAT2 texcoord;
	};

private:
	const size_t max_vertices;
	std::vector<vertex> vertices;

public:
	sprite_batch(ID3D11Device* device, const wchar_t* filename, size_t max_sprites);
	~sprite_batch();

	void render(ID3D11DeviceContext*,
		float dx, float dy,  //短形の左上の座標(スクリーン座標系)
		float dw, float dh,  //短形のサイズ(スクリーン座標系)
		float r, float g, float b, float a,
		float angle/*degree*/ //UNIT.04
	);
	void render(ID3D11DeviceContext* device_context,
		float dx, float dy, float dw, float dh,
		float r, float g, float b, float a,
		float angle/*degree*/,
		float sx, float sy, float sw, float sh
	);
	void render(ID3D11DeviceContext* device_context,
		float dx, float dy,
		float dw, float dh
	);
	void begin(ID3D11DeviceContext* device_context,
		ID3D11PixelShader* replaced_pixel_shader = nullptr,
		ID3D11ShaderResourceView* replaced_shader_resource_view = nullptr);
	void end(ID3D11DeviceContext* device_context);
private:
	static void rotate(float& x, float& y, float cx, float cy, float sin, float cos)
	{
		x -= cx;
		y -= cy;

		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;
	}
};