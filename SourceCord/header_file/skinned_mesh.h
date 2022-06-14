//UNIT17
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include <fbxsdk.h>
//UNIT.19
#include <unordered_map>
struct scene
{
	struct node
	{
		uint64_t unique_id{ 0 };
		std::string name;
		FbxNodeAttribute::EType attribute{ FbxNodeAttribute::EType::eUnknown };
		int64_t parent_index{ -1 };
	};
	std::vector<node> nodes;
	int64_t indexof(uint64_t unique_id) const
	{
		int64_t index{ 0 };
		for (const node& node : nodes)
		{
			if (node.unique_id == unique_id)
			{
				return index;
			}
			++index;
		}
		return -1;
	}
};

class skinned_mesh
{
public:
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 texcoord;
	};
	struct constants
	{
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 material_color;
	};
	//UNIT18
	struct mesh
	{
		uint64_t unique_id{ 0 };
		std::string name;
		// 'node_index' シーンのノード配列を参照するインデックスです
		int64_t node_index{ 0 };

		std::vector<vertex> vertices;
		std::vector<uint32_t> indices;
		//UNIT.20
		struct subset
		{
			uint64_t material_unique_id{ 0 };
			std::string material_name;

			uint32_t start_index_location{ 0 };
			uint32_t index_count{ 0 };
		};
		std::vector<subset> subsets;
		/*21回目*/
		DirectX::XMFLOAT4X4 DefaultGlobalTransform =
		{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
		};
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> vertex_buffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> index_buffer;
		friend class skinned_mesh;
	};
	std::vector<mesh> meshes;
	//UNIT.19
	struct material
	{
		uint64_t unique_id{ 0 };
		std::string name;

		DirectX::XMFLOAT4 Ka{ 0.2f, 0.2f, 0.2f, 1.0f };
		DirectX::XMFLOAT4 Kd{ 0.8f, 0.8f, 0.8f, 1.0f };
		DirectX::XMFLOAT4 Ks{ 1.0f, 1.0f, 1.0f, 1.0f };

		std::string texture_filenames[4];
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shader_resource_views[4];
	};
	std::unordered_map<uint64_t, material> materials;
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertex_shader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> input_layout;
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Dummy;	//マテリアルテクスチャ無し用のシェーダーリソースビュー
public:
	skinned_mesh(ID3D11Device* device, const char *fbx_filename, bool triangulate = false);
	virtual ~skinned_mesh() = default;
	void fetch_meshes(FbxScene* fbx_scene, std::vector<mesh>& meshes);
	void create_com_objects(ID3D11Device* device, const char* fbx_filename);
	void render(ID3D11DeviceContext* device_context, const DirectX::XMFLOAT4X4& world, const DirectX::XMFLOAT4& material_color);
	//UNIT.19
	void fetch_materials(FbxScene* fbx_scene, std::unordered_map<uint64_t, material>& materials);
protected:
	scene scene_view;
};