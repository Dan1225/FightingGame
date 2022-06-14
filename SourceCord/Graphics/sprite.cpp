#include "../Graphics/sprite.h"
#include "../header_file/misc.h"
#include "../header_file/texture.h"
#include "../header_file/shader.h"
#include "../Graphics/framework.h"

#include <sstream>


Sprite::Sprite(ID3D11Device* device, const wchar_t* filename)
{
	HRESULT hr{ S_OK };

	//頂点情報のセット
	vertex vertices[]
	{
		{ {-1.0,+1.0,0}, {1,1,1,1},{ 0, 0 } },
		{ {+1.0,+1.0,0}, {1,1,1,1},{ 1, 0 } },
		{ {-1.0,-1.0,0}, {1,1,1,1},{ 0, 1 } },
		{ {+1.0,-1.0,0}, {1,1,1,1},{ 1, 1 } },
	};

	//頂点情報バッファオブジェクトの生成
	D3D11_BUFFER_DESC buffer_desc{};
	buffer_desc.ByteWidth = sizeof(vertices);           //バッファのサイズ(頂点数)
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;            //使用方法
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;   //バッファの種類(頂点バッファ)
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;//UNIT.03(CPUアクセス設定)
	buffer_desc.MiscFlags = 0;                          //その他の設定
	buffer_desc.StructureByteStride = 0;                //構造体のサイズ

	//バッファの初期値設定
	D3D11_SUBRESOURCE_DATA subresource_data{};
	subresource_data.pSysMem = vertices;  //頂点バッファの初期値
	subresource_data.SysMemPitch = 0;     //頂点バッファでは使用しない
	subresource_data.SysMemSlicePitch = 0;//頂点バッファでは使用しない
	hr = device->CreateBuffer(&buffer_desc, &subresource_data, vertex_buffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	//入力レイアウトオブジェクトの生成
	D3D11_INPUT_ELEMENT_DESC input_element_desc[]
	{
		{ "POSITION",                //セマンティック名(HLSL側の名前と一致させて送信した情報を受信できる)
		0,                           //セマンティック番号(セマンティック名が同じでも識別できるようにする番号)
		DXGI_FORMAT_R32G32B32_FLOAT, //フォーマット(データの肩やバイト数の設定のことを言う 型やサイズを考えるとfloat[3]と同じ)
		0,                           //入力スロットの番号(どの入力スロットに対して反映させるかを指定する)
		D3D11_APPEND_ALIGNED_ELEMENT,//要素から先頭までのオフセット値(要素の先頭までのオフセットは各データが配列の先頭から何バイト離れているか)
		D3D11_INPUT_PER_VERTEX_DATA, //入力データの種類(頂点データとインスタンスデータの2つある今回は頂点データ)
		0                            //繰り返し回数(頂点バッファの指定なら0指定で問題なし)
		},
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,0,
		  D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0
		},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,0,
		D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0
		},
	};
	//頂点シェーダーオブジェクトの生成
	//エラーの原因はファイル指定をきっちりする(Shaders/~~)
	//自作した関数による頂点シェーダとピクセルシェーダのオブジェクトの作成
	create_vs_from_cso(device, "Shaders/sprite_vs.cso", vertex_shader.GetAddressOf(), input_layout.GetAddressOf(), input_element_desc, _countof(input_element_desc));
	create_ps_from_cso(device, "Shaders/sprite_ps.cso", pixel_shader.GetAddressOf());

	load_texture_from_file(device, filename, shader_resource_view.GetAddressOf(), &texture2d_desc);
}

Sprite::~Sprite()
{

}

void Sprite::render(
	ID3D11DeviceContext* device_context,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float angle/*degree*/,
	float r, float g, float b, float a,
	bool isTurn)
{
	//ここの実装はrender関数の先頭に実装する
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	device_context->RSGetViewports(&num_viewports, &viewport);

	//  (x0, y0) *----* (x1, y1)
	//           |   /|
	//           |  / |
	//           | /  |
	//           |/   |
	//  (x2, y2) *----* (x3, y3)

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };

#if 1
	//NDC座標への変換のあとに行うとバグるので順番に注意
	auto rotate = [](float& x, float& y, float cx, float cy, float angle)
	{
		x -= cx;
		y -= cy;

		float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
		float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;
	};
	//回転の中心を矩形の中心点にした場合
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	rotate(x0, y0, cx, cy, angle);
	rotate(x1, y1, cx, cy, angle);
	rotate(x2, y2, cx, cy, angle);
	rotate(x3, y3, cx, cy, angle);
#else
	// Translate sprite's centre to origin (rotate centre)
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	x0 -= cx;
	y0 -= cy;
	x1 -= cx;
	y1 -= cy;
	x2 -= cx;
	y2 -= cy;
	x3 -= cx;
	y3 -= cy;

	// Rotate each sprite's vertices by angle
	float tx, ty;
	float cos{ cosf(XMConvertToRadians(angle)) };
	float sin{ sinf(XMConvertToRadians(angle)) };
	tx = x0;
	ty = y0;
	x0 = cos * tx + -sin * ty;
	y0 = sin * tx + cos * ty;
	tx = x1;
	ty = y1;
	x1 = cos * tx + -sin * ty;
	y1 = sin * tx + cos * ty;
	tx = x2;
	ty = y2;
	x2 = cos * tx + -sin * ty;
	y2 = sin * tx + cos * ty;
	tx = x3;
	ty = y3;
	x3 = cos * tx + -sin * ty;
	y3 = sin * tx + cos * ty;

	// Translate sprite's centre to original position
	x0 += cx;
	y0 += cy;
	x1 += cx;
	y1 += cy;
	x2 += cx;
	y2 += cy;
	x3 += cx;
	y3 += cy;
#endif
	//スクリーン座標系からNDCへの座標変換を行う
	x0 = 2.0f * x0 / viewport.Width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / viewport.Height;
	x1 = 2.0f * x1 / viewport.Width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / viewport.Height;
	x2 = 2.0f * x2 / viewport.Width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / viewport.Height;
	x3 = 2.0f * x3 / viewport.Width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / viewport.Height;

	//計算結果で頂点バッファオブジェクトを更新する
	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,0 };
		vertices[1].position = { x1,y1,0 };
		vertices[2].position = { x2,y2,0 };
		vertices[3].position = { x3,y3,0 };
		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

		if (isTurn == false)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
		}
		if(isTurn == true)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
		}
	}

	device_context->Unmap(vertex_buffer.Get(), 0);

	//頂点バッファのバインド
	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//プリミティブタイプおよびデータの順序に関する情報のバインド
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//入力レイアウトオブジェクトのバインド
	device_context->IASetInputLayout(input_layout.Get());

	//シェーダーのバインド
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	//シェーダーリソースのバインド
	device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

	//プリミティブの描画
	device_context->Draw(4, 0);
}

void Sprite::render(
	ID3D11DeviceContext* device_context,
	float dx, float dy, float dw, float dh,
	float sx, float sy, float sw, float sh,
	float angle/*degree*/,
	Color color,
	bool isTurn)
{
	//ここの実装はrender関数の先頭に実装する
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	device_context->RSGetViewports(&num_viewports, &viewport);

	//  (x0, y0) *----* (x1, y1)
	//           |   /|
	//           |  / |
	//           | /  |
	//           |/   |
	//  (x2, y2) *----* (x3, y3)

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };

#if 1
	//NDC座標への変換のあとに行うとバグるので順番に注意
	auto rotate = [](float& x, float& y, float cx, float cy, float angle)
	{
		x -= cx;
		y -= cy;

		float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
		float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;
	};
	//回転の中心を矩形の中心点にした場合
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	rotate(x0, y0, cx, cy, angle);
	rotate(x1, y1, cx, cy, angle);
	rotate(x2, y2, cx, cy, angle);
	rotate(x3, y3, cx, cy, angle);
#else
	// Translate sprite's centre to origin (rotate centre)
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	x0 -= cx;
	y0 -= cy;
	x1 -= cx;
	y1 -= cy;
	x2 -= cx;
	y2 -= cy;
	x3 -= cx;
	y3 -= cy;

	// Rotate each sprite's vertices by angle
	float tx, ty;
	float cos{ cosf(XMConvertToRadians(angle)) };
	float sin{ sinf(XMConvertToRadians(angle)) };
	tx = x0;
	ty = y0;
	x0 = cos * tx + -sin * ty;
	y0 = sin * tx + cos * ty;
	tx = x1;
	ty = y1;
	x1 = cos * tx + -sin * ty;
	y1 = sin * tx + cos * ty;
	tx = x2;
	ty = y2;
	x2 = cos * tx + -sin * ty;
	y2 = sin * tx + cos * ty;
	tx = x3;
	ty = y3;
	x3 = cos * tx + -sin * ty;
	y3 = sin * tx + cos * ty;

	// Translate sprite's centre to original position
	x0 += cx;
	y0 += cy;
	x1 += cx;
	y1 += cy;
	x2 += cx;
	y2 += cy;
	x3 += cx;
	y3 += cy;
#endif
	//スクリーン座標系からNDCへの座標変換を行う
	x0 = 2.0f * x0 / viewport.Width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / viewport.Height;
	x1 = 2.0f * x1 / viewport.Width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / viewport.Height;
	x2 = 2.0f * x2 / viewport.Width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / viewport.Height;
	x3 = 2.0f * x3 / viewport.Width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / viewport.Height;

	//計算結果で頂点バッファオブジェクトを更新する
	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,0 };
		vertices[1].position = { x1,y1,0 };
		vertices[2].position = { x2,y2,0 };
		vertices[3].position = { x3,y3,0 };
		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { color.r,color.g,color.b,color.a };

		if (isTurn == false)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
		}
		if (isTurn == true)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(sy) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(sx + sw) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(sx) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(sy + sh) / texture2d_desc.Height;
		}
	}

	device_context->Unmap(vertex_buffer.Get(), 0);

	//頂点バッファのバインド
	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//プリミティブタイプおよびデータの順序に関する情報のバインド
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//入力レイアウトオブジェクトのバインド
	device_context->IASetInputLayout(input_layout.Get());

	//シェーダーのバインド
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	//シェーダーリソースのバインド
	device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

	//プリミティブの描画
	device_context->Draw(4, 0);
}

//アニメーション付きの場合に使用
void Sprite::render(ID3D11DeviceContext* device_context,
	float dx, float dy, float dw, float dh,
	const Rect& rect,
	float angle/*degree*/,
	float r, float g, float b, float a,
	bool isTurn)
{
	//ここの実装はrender関数の先頭に実装する
	D3D11_VIEWPORT viewport{};
	UINT num_viewports{ 1 };
	device_context->RSGetViewports(&num_viewports, &viewport);

	//  (x0, y0) *----* (x1, y1)
	//           |   /|
	//           |  / |
	//           | /  |
	//           |/   |
	//  (x2, y2) *----* (x3, y3)

	//left-top
	float x0{ dx };
	float y0{ dy };
	//right-top
	float x1{ dx + dw };
	float y1{ dy };
	//left-bottom
	float x2{ dx };
	float y2{ dy + dh };
	//right-bottom
	float x3{ dx + dw };
	float y3{ dy + dh };

#if 1
	//NDC座標への変換のあとに行うとバグるので順番に注意
	auto rotate = [](float& x, float& y, float cx, float cy, float angle)
	{
		x -= cx;
		y -= cy;

		float cos{ cosf(DirectX::XMConvertToRadians(angle)) };
		float sin{ sinf(DirectX::XMConvertToRadians(angle)) };
		float tx{ x }, ty{ y };
		x = cos * tx + -sin * ty;
		y = sin * tx + cos * ty;

		x += cx;
		y += cy;
	};
	//回転の中心を矩形の中心点にした場合
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	rotate(x0, y0, cx, cy, angle);
	rotate(x1, y1, cx, cy, angle);
	rotate(x2, y2, cx, cy, angle);
	rotate(x3, y3, cx, cy, angle);
#else
	// Translate sprite's centre to origin (rotate centre)
	float cx = dx + dw * 0.5f;
	float cy = dy + dh * 0.5f;
	x0 -= cx;
	y0 -= cy;
	x1 -= cx;
	y1 -= cy;
	x2 -= cx;
	y2 -= cy;
	x3 -= cx;
	y3 -= cy;

	// Rotate each sprite's vertices by angle
	float tx, ty;
	float cos{ cosf(XMConvertToRadians(angle)) };
	float sin{ sinf(XMConvertToRadians(angle)) };
	tx = x0;
	ty = y0;
	x0 = cos * tx + -sin * ty;
	y0 = sin * tx + cos * ty;
	tx = x1;
	ty = y1;
	x1 = cos * tx + -sin * ty;
	y1 = sin * tx + cos * ty;
	tx = x2;
	ty = y2;
	x2 = cos * tx + -sin * ty;
	y2 = sin * tx + cos * ty;
	tx = x3;
	ty = y3;
	x3 = cos * tx + -sin * ty;
	y3 = sin * tx + cos * ty;

	// Translate sprite's centre to original position
	x0 += cx;
	y0 += cy;
	x1 += cx;
	y1 += cy;
	x2 += cx;
	y2 += cy;
	x3 += cx;
	y3 += cy;
#endif
	//スクリーン座標系からNDCへの座標変換を行う
	x0 = 2.0f * x0 / viewport.Width - 1.0f;
	y0 = 1.0f - 2.0f * y0 / viewport.Height;
	x1 = 2.0f * x1 / viewport.Width - 1.0f;
	y1 = 1.0f - 2.0f * y1 / viewport.Height;
	x2 = 2.0f * x2 / viewport.Width - 1.0f;
	y2 = 1.0f - 2.0f * y2 / viewport.Height;
	x3 = 2.0f * x3 / viewport.Width - 1.0f;
	y3 = 1.0f - 2.0f * y3 / viewport.Height;

	//計算結果で頂点バッファオブジェクトを更新する
	HRESULT hr{ S_OK };
	D3D11_MAPPED_SUBRESOURCE mapped_subresource{};
	hr = device_context->Map(vertex_buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
	_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

	vertex* vertices{ reinterpret_cast<vertex*>(mapped_subresource.pData) };
	if (vertices != nullptr)
	{
		vertices[0].position = { x0,y0,0 };
		vertices[1].position = { x1,y1,0 };
		vertices[2].position = { x2,y2,0 };
		vertices[3].position = { x3,y3,0 };

		vertices[0].color = vertices[1].color = vertices[2].color = vertices[3].color = { r,g,b,a };

		if (isTurn == false)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(rect.x) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(rect.y) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(rect.x + rect.w) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(rect.y) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(rect.x) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(rect.y + rect.h) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(rect.x+ rect.w) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(rect.y + rect.h) / texture2d_desc.Height;
		}
		if (isTurn == true)
		{
			vertices[0].texcoord.x = static_cast<FLOAT>(rect.x + rect.w) / texture2d_desc.Width;
			vertices[0].texcoord.y = static_cast<FLOAT>(rect.y) / texture2d_desc.Height;
			vertices[1].texcoord.x = static_cast<FLOAT>(rect.x) / texture2d_desc.Width;
			vertices[1].texcoord.y = static_cast<FLOAT>(rect.y) / texture2d_desc.Height;
			vertices[2].texcoord.x = static_cast<FLOAT>(rect.x + rect.w) / texture2d_desc.Width;
			vertices[2].texcoord.y = static_cast<FLOAT>(rect.y + rect.h) / texture2d_desc.Height;
			vertices[3].texcoord.x = static_cast<FLOAT>(rect.x) / texture2d_desc.Width;
			vertices[3].texcoord.y = static_cast<FLOAT>(rect.y + rect.h) / texture2d_desc.Height;
		}
	}

	device_context->Unmap(vertex_buffer.Get(), 0);

	//頂点バッファのバインド
	UINT stride{ sizeof(vertex) };
	UINT offset{ 0 };
	device_context->IASetVertexBuffers(0, 1, vertex_buffer.GetAddressOf(), &stride, &offset);

	//プリミティブタイプおよびデータの順序に関する情報のバインド
	device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//入力レイアウトオブジェクトのバインド
	device_context->IASetInputLayout(input_layout.Get());

	//シェーダーのバインド
	device_context->VSSetShader(vertex_shader.Get(), nullptr, 0);
	device_context->PSSetShader(pixel_shader.Get(), nullptr, 0);

	//シェーダーリソースのバインド
	device_context->PSSetShaderResources(0, 1, shader_resource_view.GetAddressOf());

	//プリミティブの描画
	device_context->Draw(4, 0);
}

//文字表示関数
void Sprite::textout(ID3D11DeviceContext* device_context, std::string s, float x, float y, float w, float h, float r, float g, float b, float a)
{
	float sw = static_cast<float>(texture2d_desc.Width / 16);
	float sh = static_cast<float>(texture2d_desc.Height / 16);
	float carriage = 0;
	for (const char c : s)
	{
		render(device_context,
			x + carriage, y, w, h,
			sw * (c & 0x0F), sh * (c >> 4), sw, sh,
			0,
			r, g, b, a);
		carriage += w;
	}
}