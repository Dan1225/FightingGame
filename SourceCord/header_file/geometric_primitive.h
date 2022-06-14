//UNIT.11
#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
using namespace DirectX;
#include <wrl.h>
using namespace Microsoft::WRL;
class geometric_primitive
{
public:
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
	};
	struct constants
	{
		XMFLOAT4X4 world;
		XMFLOAT4 material_color;
	};

private:
	ComPtr<ID3D11Buffer> vertex_buffer;
	ComPtr<ID3D11Buffer> index_buffer;

	ComPtr<ID3D11VertexShader> vertex_shader;
	ComPtr<ID3D11PixelShader>  pixel_shader;
	ComPtr<ID3D11InputLayout>  input_layout;
	ComPtr<ID3D11Buffer>       constant_buffer;

	//UNIT.12
	ComPtr<ID3D11RasterizerState> rasterizer_states[3];

public:
	geometric_primitive(ID3D11Device* device);
	virtual ~geometric_primitive() = default;

	void render(ID3D11DeviceContext* device_context,
	  const DirectX::XMFLOAT4X4 & world, const DirectX::XMFLOAT4& material_color, bool wireframe = false);

protected:
	void create_com_buffers(ID3D11Device* device, vertex* vertices, size_t vertex_count,
		uint32_t* indices, size_t index_count);
};

class geometric_cube : public geometric_primitive
{
public:
	geometric_cube(ID3D11Device* device);
};

class geometric_cylinder : public geometric_primitive
{
public:
	geometric_cylinder(ID3D11Device* device, u_int slices);
};

class geometric_sphere : public geometric_primitive
{
public:
	geometric_sphere(ID3D11Device* device, u_int slices, u_int stacks);
};

class geometric_capsule : public geometric_primitive
{
public:
	geometric_capsule(ID3D11Device* device);
};