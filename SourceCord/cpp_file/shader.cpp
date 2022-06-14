#include "../header_file/shader.h"
#include "../header_file/misc.h"

#include <memory>
using namespace std;

HRESULT create_vs_from_cso(
	ID3D11Device* device,
	const char* cso_name,
	ID3D11VertexShader** vertex_shader,
	ID3D11InputLayout** input_layout,
	D3D11_INPUT_ELEMENT_DESC* input_element_desc,
	UINT num_elements)
{
	FILE* fp{ nullptr };
	fopen_s(&fp, cso_name, "rb"); //�������Ƀt�@�C���^�̃|�C���^�ϐ��̃A�h���X���w�肷�邱�ƂŁA
								  //�������Ŏw�肵���t�@�C���� ��O�����őI�񂾃��[�h�ŊJ���A
								  //���̊J�����t�@�C���̃A�h���X��������Ŏw�肵���|�C���^�ϐ��ɓn���B
	_ASSERT_EXPR_A(fp, "CSO File not found");

	//fseek�֐��̓t�@�C���|�C���^�̈ʒu���w�肵���ʒu�Ɉړ������邱�Ƃ��ł���֐��ł��B
	fseek(fp, 0, SEEK_END);  //�������̓t�@�C���|�C���^
							 //�������͂ǂꂾ���ړ������邩�Ƃ����I�t�Z�b�g�l
							 //��O�����͂ǂ̈ʒu����ړ������邩�Ƃ����萔
	long cso_sz{ ftell(fp) };//���݊J���Ă���t�@�C���|�C���^�̃o�C�g�P�ʂ̈ʒu��ԋp����B
	fseek(fp, 0, SEEK_SET);  //SEEK_SET�@�i�t�@�C���̐擪����j
							 //SEEK_CUR�@�i�t�@�C���̌��݈ʒu����j
							 //SEEK_END�@�i�t�@�C���̏I�[����j

	unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
	//fread�֐��̓t�@�C������w��o�C�g���̃f�[�^���w�肵�����ǂݍ��݁A�o�b�t�@�Ɋi�[���܂��B
	fread(cso_data.get(), cso_sz, 1, fp);//�������ɂ͊i�[��̃o�b�t�@
										 //�������͓ǂݍ��ރf�[�^�P�̃o�C�g��
										 //��O�����͓ǂݍ��ރf�[�^�̌�
										 //��l�����̓t�@�C���|�C���^
	fclose(fp);//fopen�̖߂�l�Ƃ��Ď擾�����t�@�C���|�C���^�������ɓn���ăN���[�Y����B

	HRESULT hr{ S_OK };
	hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);//���_�V�F�[�_�̐���
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	if (input_layout)
	{
		hr = device->CreateInputLayout(input_element_desc, num_elements, cso_data.get(), cso_sz, input_layout);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	return hr;
}

HRESULT create_ps_from_cso(
	ID3D11Device* device,
	const char* cso_name,
	ID3D11PixelShader** pixel_shader)
{
	FILE* fp{ nullptr };
	fopen_s(&fp, cso_name, "rb");
	_ASSERT_EXPR_A(fp, "CSO File not found");

	fseek(fp, 0, SEEK_END);
	long cso_sz{ ftell(fp) };
	fseek(fp, 0, SEEK_SET);

	unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
	fread(cso_data.get(), cso_sz, 1, fp);
	fclose(fp);

	HRESULT hr{ S_OK };
	hr = device->CreatePixelShader(cso_data.get(), cso_sz, nullptr, pixel_shader);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	return hr;
}
