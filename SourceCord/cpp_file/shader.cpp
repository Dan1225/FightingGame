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
	fopen_s(&fp, cso_name, "rb"); //第一引数にファイル型のポインタ変数のアドレスを指定することで、
								  //第二引数で指定したファイルを 第三引数で選んだモードで開き、
								  //その開いたファイルのアドレスを第一引数で指定したポインタ変数に渡す。
	_ASSERT_EXPR_A(fp, "CSO File not found");

	//fseek関数はファイルポインタの位置を指定した位置に移動させることができる関数です。
	fseek(fp, 0, SEEK_END);  //第一引数はファイルポインタ
							 //第二引数はどれだけ移動させるかというオフセット値
							 //第三引数はどの位置から移動させるかという定数
	long cso_sz{ ftell(fp) };//現在開いているファイルポインタのバイト単位の位置を返却する。
	fseek(fp, 0, SEEK_SET);  //SEEK_SET　（ファイルの先頭から）
							 //SEEK_CUR　（ファイルの現在位置から）
							 //SEEK_END　（ファイルの終端から）

	unique_ptr<unsigned char[]> cso_data{ make_unique<unsigned char[]>(cso_sz) };
	//fread関数はファイルから指定バイト数のデータを指定した数読み込み、バッファに格納します。
	fread(cso_data.get(), cso_sz, 1, fp);//第一引数には格納先のバッファ
										 //第二引数は読み込むデータ１つのバイト数
										 //第三引数は読み込むデータの個数
										 //第四引数はファイルポインタ
	fclose(fp);//fopenの戻り値として取得したファイルポインタを引数に渡してクローズする。

	HRESULT hr{ S_OK };
	hr = device->CreateVertexShader(cso_data.get(), cso_sz, nullptr, vertex_shader);//頂点シェーダの生成
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
