#include "../header_file/misc.h"
#include "../Graphics/Graphics.h"

class Primitive;
Graphics* Graphics::instance = nullptr;

//コンストラクタ
Graphics::Graphics(HWND hwnd)
{
	instance = this;

	HRESULT hr{ S_OK };

	UINT create_device_flags{ 0 };
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 };
	//スワップチェーンの作成
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain_desc.BufferCount = 1;                               //スワップチェインのバッファ数
		swap_chain_desc.BufferDesc.Width = screenWidth;                //バッファの横幅
		swap_chain_desc.BufferDesc.Height = screenHeight;              //バッファの縦幅
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//カラーフォーマット
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;         //リフレッシュレートの分母
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;        //リフレッシュレートの分子
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //バッファの使い方 Usage => 使用方法
		swap_chain_desc.OutputWindow = hwnd;                           //出力対象のウィドウハンドル
		swap_chain_desc.SampleDesc.Count = 1;                          //マルチサンプリングのサンプル方法(未使用は1)
		swap_chain_desc.SampleDesc.Quality = 0;                        //マルチサンプリングの品質(未使用は0)
		swap_chain_desc.Windowed = !FULLSCREEN;                        //ウィンドウモード指定
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,                          //ビデオアダプタ指定(nullptrは既定)
			D3D_DRIVER_TYPE_HARDWARE,      //ドライバのタイプ
			NULL,                          //D3D_DRIVER_TYPE_SOFTWARE指定時に使用
			create_device_flags,           //フラグ指定
			&feature_levels,               //D3D_FEATURE_LEVEL指定で自分で定義した配列を指定可能
			1,                             //上のD3D_FEATURE_LEVEL配列の要素数
			D3D11_SDK_VERSION,             //SDKバージョン
			&swap_chain_desc,              //DXGI_SWAP_CHAIN_DESC
			swap_chain.GetAddressOf(),     //関数実行時のSwapChainの出力先
			device.GetAddressOf(),         //関数実行時にのDeviceの出力先
			NULL,                          //成功したD3DFEATURE_LEVELの出力先
			device_context.GetAddressOf());//関数成功時のCotextの出力先
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	//レンダーターゲットビューの作成
	{
		ID3D11Texture2D* back_buffer{};
		hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&back_buffer));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = device->CreateRenderTargetView(back_buffer, NULL, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		back_buffer->Release();

		//深度ステンシルビューの作成
		ID3D11Texture2D* depth_stencil_buffer{};
		D3D11_TEXTURE2D_DESC texture2d_desc{};
		texture2d_desc.Width = screenWidth; //横幅
		texture2d_desc.Height = screenHeight;//縦幅
		//ミップマップのレベル指定
		//どのレベルまで生成するかという設定
		//1はマルチサンプリングされたテクスチャを使用するあるので、
		//ミップマップはなしと考えられる
		//0は全生成とあるので可能な限りのレベルまでテクスチャが生成されると思われる
		texture2d_desc.MipLevels = 1;
		//テクスチャ配列のサイズ指定
		//テクスチャ配列について調べ切れていないので
		//他のサンプルと同様に1を設定しておく
		texture2d_desc.ArraySize = 1;
		//テクスチャのフォーマット
		//DXGI_FORMAT_D24_UNORM_S8_UINTを使用する
		//これはおそらくDepth24bit、Stencil8bitとされると思う
		//※．UNORMはUnsigned NORMalizedの略で指定された範囲を0.0～1.0にするらしい
		texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//テクスチャーフォーマット
		//マルチサンプリング設定
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;           //テクスチャの使用方法(デフォルト)
		texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;  //Bind設定はDepth,Stencilに設定
		//リソースへのCPUのアクセス権限についての設定
		//ReadとWriteがあるが、権限について現状は考える必要はないはずなので、
		//デフォルト値であると思われる0をしておく
		//※readとwriteはenum(D3D11_CPU_ACCESS_FLAG)で用意されていた
		texture2d_desc.CPUAccessFlags = 0;
		//リソースオプションのフラグ
		//Microsoft Docsでフラグを確認する限りは
		//通常使用でフラグを設定する必要はないと思われるので
		//0としておく
		texture2d_desc.MiscFlags = 0;
		//texture2d_descの情報でテクスチャを作成
		hr = device->CreateTexture2D(&texture2d_desc, NULL, &depth_stencil_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
		depth_stencil_view_desc.Format = texture2d_desc.Format;// Viewのフォーマット(Textureのものを使用)
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0; //最初に使用するミップマップのレベルを指定
		hr = device->CreateDepthStencilView(
			depth_stencil_buffer,            //DSVとして使用されるTexture
			&depth_stencil_view_desc,        //DSVの設定
			depth_stencil_view.GetAddressOf()//ID3D11DepthStencilViewの出力先
		);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		depth_stencil_buffer->Release();
	}
	//ビューポートの設定
	{
		D3D11_VIEWPORT viewport{};
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = static_cast<float>(screenWidth);
		viewport.Height = static_cast<float>(screenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		device_context->RSSetViewports(1, &viewport);
	}
	//サンプラーステート
	{
		D3D11_SAMPLER_DESC sampler_desc;
		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampler_desc.MipLODBias = 0;
		sampler_desc.MaxAnisotropy = 16;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0;
		sampler_desc.BorderColor[1] = 0;
		sampler_desc.BorderColor[2] = 0;
		sampler_desc.BorderColor[3] = 0;
		sampler_desc.MinLOD = 0;
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = device->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::POINT)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		hr = device->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::LINEAR)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		sampler_desc.Filter = D3D11_FILTER_ANISOTROPIC;
		hr = device->CreateSamplerState(&sampler_desc, sampler_states[static_cast<size_t>(SAMPLER_STATE::ANISOTROPIC)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		//深度テスト:オン 深度ライト:オン
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;                         //深度テストを行うかのテスト.TRUEでテストをする
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//深度テストを行う際の元データと上書きするデータ同士の比較方法の指定
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //深度データを書き込む際のマスクの設定
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//深度テスト:オン 深度ライト:オフ
		depth_stencil_desc.DepthEnable = TRUE;                         //深度テストを行うかのテスト.TRUEでテストをする
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//深度テストを行う際の元データと上書きするデータ同士の比較方法の指定
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //深度データを書き込む際のマスクの設定
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//深度テスト:オフ 深度ライト:オン
		depth_stencil_desc.DepthEnable = FALSE;                         //深度テストを行うかのテスト.TRUEでテストをする
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//深度テストを行う際の元データと上書きするデータ同士の比較方法の指定
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //深度データを書き込む際のマスクの設定
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//深度テスト:オフ 深度ライト:オフ
		depth_stencil_desc.DepthEnable = FALSE;                         //深度テストを行うかのテスト.TRUEでテストをする
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//深度テストを行う際の元データと上書きするデータ同士の比較方法の指定
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //深度データを書き込む際のマスクの設定
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	{
		D3D11_BUFFER_DESC buffer_desc{};
		buffer_desc.ByteWidth = sizeof(scene_constants);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = device->CreateBuffer(&buffer_desc, nullptr, constant_buffers[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
}

// デストラクタ
Graphics::~Graphics()
{
}

