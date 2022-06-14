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
			// �􉽊w�I�Ȓ��_�Ƃ���x y z���W���w�肷��D
			// �L���Ȑ���Ȗʂɂ́C4�Ԗڂ̓������W���K�v�ł���D
			// �d�݂ƌĂ΂��D
			//
			// x y z �́C���_�� x, y, z ���W�ł���D������,
			// 3�����ɂ����钸�_�̈ʒu���`���镂�������_���ł��D�����͕��������_���ł���C
			// // 3�����ɂ����钸�_�̈ʒu���`����D
			//
			// w �́C�L���Ȑ�����ыȖʂɕK�v�ȏd�݂ŁC��L���Ȑ�����ыȖʂɂ�
			// // �K�v����܂���D��L���Ȑ��ƃT�[�t�F�X�ɂ͕K�v����܂���B
			// w�ɒl���w�肵�Ȃ��ꍇ�A�f�t�H���g��1.0�ł�
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
			// �e�N�X�`���̒��_�Ƃ��̍��W���w�肵�܂��D1D �e�N�X�`���� u �e�N�X�`�����W�݂̂�K�v�Ƃ��C
			// 2D �e�N�X�`���� u �� v �e�N�X�`�����W�̗�����K�v�Ƃ��܂��D
			// �� v �̃e�N�X�`�����W���K�v�ł��B
			// ���W���w�肵�܂��B
			//
			// u �̓e�N�X�`���̐���������\���l�ł��B
			// v�̓I�v�V�����̈����ł��B
			// v �̓e�N�X�`���̐��������̒l�ł��B
			// �f�t�H���g�� 0 �ł��B
			// w �̓I�v�V�����̈����ł��B
			// w�̓e�N�X�`���̐[����\���l�ł��B�f�t�H���g��0�ł��B
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
		// ����i�Aj�Ak�̖@���x�N�g�����w�肵�܂��B
		//
		// ���_�@���́C�W�I���g���̃X���[�Y�ȃV�F�[�f�B���O�ƃ����_�����O�ɉe����^���܂��B
		// �|���S���̏ꍇ�C���_�@���͎��ۂ̃t�@�Z�b�g�@���̑���Ɏg�p�����B
		// �|���S���̏ꍇ�A���_�@���͎��ۂ̃t�@�Z�b�g�@���̑���Ɏg�p����܂��B // �T�[�t�F�X�̏ꍇ�A���_�@���̓T�[�t�F�X�S�̂ɓn���ĕ�Ԃ���
		// �T�[�t�F�X�̏ꍇ�A���_�@���̓T�[�t�F�X�S�̂ɂ킽���ĕ�Ԃ���A���ۂ̉�͓I�ȃT�[�t�F�X�@���ɒu���������܂��B
		//
		// ���_�@�������݂���ꍇ�́A�X���[�W���O�O���[�v�����D�悳��܂��B
		//
		// i j k �́C���_�@���� i, j, k ���W�ł��D
		// �͕��������_���ł��B
			FLOAT i, j, k;
			fin >> i >> j >> k;
			normals.push_back({ i, j, k });
			fin.ignore(1024, L'\n');
		}
		else if (0 == wcscmp(command, L"f"))
		{
			//f v1 / vt1 / vn1 v2 / vt2 / vn2 v3 / vt3 / vn3 ... ...
			//
			// �I�v�V�����Ƃ��āA�e�N�X�`���̒��_�ƒ��_�̖@���̎Q�Ɣԍ����܂߂邱�Ƃ��ł��܂��B
			// ���l
			// ���_�A�e�N�X�`�����_�A����ђ��_�@���̎Q�Ɣԍ��́A // �K�v�ł��B���_�A�e�N�X�`�����_�A���_�@���̎Q�Ɣԍ���
			// // �X���b�V���i/�j�ŋ�؂�Ȃ���΂Ȃ�܂���B
			// �ԍ��ƃX���b�V���̊Ԃɂ̓X�y�[�X�͂���܂���B
			// v �͖ʂ̗v�f�̒��_�̎Q�Ɣԍ��ł��B
			// �Œ�3�̒��_���K�v�ł��D
			// vt �̓I�v�V�����̈����ł��D
			// vt �� face �v�f���̃e�N�X�`�����_�̎Q�Ɣԍ��ł��B
			// ����͏�ɍŏ��̃X���b�V���̌�ɑ����܂��B
			// vn �̓I�v�V�����̈����ł��B
			// vn��face�v�f���̒��_�@���̎Q�Ɣԍ��ł��B
			// ����͏��2�Ԗڂ̃X���b�V���̌�ɑ����K�v������܂��D
			// �ʂ̗v�f�͖ʂ̖@�����g���Ă��̕����������܂��B
			// ���_���ʂ𒆐S�ɔ����v���ɕ���ł���ꍇ�A�ʂƖ@���̗�����
			// ���_����̎���Ŕ����v���ɕ���ł���ꍇ�A��Ɩ@���̗������r���[�A�̕��������܂��B
			// ���_�̏��������v���̏ꍇ�A�����Ƃ��r���[�A�[���痣�ꂽ�Ƃ�����w���܂��B
			//���_�@�������蓖�Ă��Ă���ꍇ�A�����͕\�ʖ@���̈�ʓI�ȕ����������Ă��Ȃ���΂Ȃ�܂���B
			// �\���ł��Ȃ����ʂ�������\��������܂��B
			// �ʂɃe�N�X�`���}�b�v�����蓖�Ă��Ă��āAf���Ńe�N�X�`�����_�����蓖�Ă��Ă��Ȃ��ꍇ��
			// �ʂɃe�N�X�`���}�b�v�����蓖�Ă��Ă��āAf�X�e�[�g�����g�Ńe�N�X�`�����_�����蓖�Ă��Ă��Ȃ��ꍇ�A�v�f�̃����_�����O���Ƀe�N�X�`���}�b�v�͖�������܂��B
			// �v�f�������_�����O�����Ƃ��A�e�N�X�`���}�b�v�͖�������܂��B
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
			// mtllib �t�@�C����1 �t�@�C����2 . . .
			// usemtl�X�e�[�g�����g�Őݒ肳�ꂽ�ޗ���`�̍ޗ����C�u�����t�@�C�����w�肵�܂��B
			// mtllib�ł͕����̃t�@�C�������w�肷�邱�Ƃ��ł��܂��B
			// �����̃t�@�C�������w�肳�ꂽ�ꍇ�́A�ŏ��Ƀ��X�g�A�b�v���ꂽ�t�@�C������ޗ���`����������܂��B
			// �����̃t�@�C�������w�肳�ꂽ�ꍇ�A�ŏ��ɍޗ���`����������A����2�Ԗڂ̃t�@
			// �t�@�C�����𕡐��w�肷�邱�Ƃ��ł��܂��B
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
			/*newmtl�܂œǂ񂾂�A���̂��Ƃɂ����K�p���Ă�}�e���A���̖��O������
			�����NewMtl�Ɋi�[���āA�w�b�_�[�ō�����z��̒���Name�ɕۑ�*/
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
	//�e�N�X�`���̃��[�h
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
		// device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());//�e�N�X�`�������f���ꂽ
		// UNIT.15
		device_context->PSSetShaderResources(0, 1, material.shader_resource_views[0].GetAddressOf());//�e�N�X�`�������f���ꂽ
		device_context->PSSetShaderResources(1, 1, material.shader_resource_views[1].GetAddressOf());//�e�N�X�`�������f���ꂽ


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