//UNIT.02
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
//å¬êlìIÇ…í«â¡ÇµÇΩÇ‡ÇÃ
#include<wrl.h>
#include <string>

typedef struct Rect
{
	float x, y;
	float w, h;
} Rect;

typedef struct Color
{
	float r, g, b, a;
} Color;

class Sprite
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>  pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>       vertex_buffer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_view;
	D3D11_TEXTURE2D_DESC texture2d_desc;

public:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

	Sprite(ID3D11Device* device, const wchar_t* filename);

	virtual ~Sprite();

	void render(ID3D11DeviceContext* device_context,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle/*degree*/,
		float r, float g, float b, float a,
		bool isTurn = false);

	void render(ID3D11DeviceContext* device_context,
		float dx, float dy, float dw, float dh,
		float sx, float sy, float sw, float sh,
		float angle/*degree*/,
		Color color,
		bool isTurn = false);

	void render(ID3D11DeviceContext* device_context,
		float dx, float dy, float dw, float dh,
		const Rect& rect,
		float angle/*degree*/,
		float r, float g, float b, float a,
		bool isTurn = false);

	void textout(ID3D11DeviceContext* device_context,
		std::string s,
		float x, float y,
		float w, float h,
		float r, float g, float b, float a);
};