#include "../header_file/misc.h"
#include "../Graphics/Graphics.h"

class Primitive;
Graphics* Graphics::instance = nullptr;

//�R���X�g���N�^
Graphics::Graphics(HWND hwnd)
{
	instance = this;

	HRESULT hr{ S_OK };

	UINT create_device_flags{ 0 };
#ifdef _DEBUG
	create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 };
	//�X���b�v�`�F�[���̍쐬
	{
		DXGI_SWAP_CHAIN_DESC swap_chain_desc{};
		swap_chain_desc.BufferCount = 1;                               //�X���b�v�`�F�C���̃o�b�t�@��
		swap_chain_desc.BufferDesc.Width = screenWidth;                //�o�b�t�@�̉���
		swap_chain_desc.BufferDesc.Height = screenHeight;              //�o�b�t�@�̏c��
		swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�J���[�t�H�[�}�b�g
		swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;         //���t���b�V�����[�g�̕���
		swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;        //���t���b�V�����[�g�̕��q
		swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //�o�b�t�@�̎g���� Usage => �g�p���@
		swap_chain_desc.OutputWindow = hwnd;                           //�o�͑Ώۂ̃E�B�h�E�n���h��
		swap_chain_desc.SampleDesc.Count = 1;                          //�}���`�T���v�����O�̃T���v�����@(���g�p��1)
		swap_chain_desc.SampleDesc.Quality = 0;                        //�}���`�T���v�����O�̕i��(���g�p��0)
		swap_chain_desc.Windowed = !FULLSCREEN;                        //�E�B���h�E���[�h�w��
		hr = D3D11CreateDeviceAndSwapChain(
			NULL,                          //�r�f�I�A�_�v�^�w��(nullptr�͊���)
			D3D_DRIVER_TYPE_HARDWARE,      //�h���C�o�̃^�C�v
			NULL,                          //D3D_DRIVER_TYPE_SOFTWARE�w�莞�Ɏg�p
			create_device_flags,           //�t���O�w��
			&feature_levels,               //D3D_FEATURE_LEVEL�w��Ŏ����Œ�`�����z����w��\
			1,                             //���D3D_FEATURE_LEVEL�z��̗v�f��
			D3D11_SDK_VERSION,             //SDK�o�[�W����
			&swap_chain_desc,              //DXGI_SWAP_CHAIN_DESC
			swap_chain.GetAddressOf(),     //�֐����s����SwapChain�̏o�͐�
			device.GetAddressOf(),         //�֐����s���ɂ�Device�̏o�͐�
			NULL,                          //��������D3DFEATURE_LEVEL�̏o�͐�
			device_context.GetAddressOf());//�֐���������Cotext�̏o�͐�
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

	//�����_�[�^�[�Q�b�g�r���[�̍쐬
	{
		ID3D11Texture2D* back_buffer{};
		hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&back_buffer));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = device->CreateRenderTargetView(back_buffer, NULL, render_target_view.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		back_buffer->Release();

		//�[�x�X�e���V���r���[�̍쐬
		ID3D11Texture2D* depth_stencil_buffer{};
		D3D11_TEXTURE2D_DESC texture2d_desc{};
		texture2d_desc.Width = screenWidth; //����
		texture2d_desc.Height = screenHeight;//�c��
		//�~�b�v�}�b�v�̃��x���w��
		//�ǂ̃��x���܂Ő������邩�Ƃ����ݒ�
		//1�̓}���`�T���v�����O���ꂽ�e�N�X�`�����g�p���邠��̂ŁA
		//�~�b�v�}�b�v�͂Ȃ��ƍl������
		//0�͑S�����Ƃ���̂ŉ\�Ȍ���̃��x���܂Ńe�N�X�`�������������Ǝv����
		texture2d_desc.MipLevels = 1;
		//�e�N�X�`���z��̃T�C�Y�w��
		//�e�N�X�`���z��ɂ��Ē��א؂�Ă��Ȃ��̂�
		//���̃T���v���Ɠ��l��1��ݒ肵�Ă���
		texture2d_desc.ArraySize = 1;
		//�e�N�X�`���̃t�H�[�}�b�g
		//DXGI_FORMAT_D24_UNORM_S8_UINT���g�p����
		//����͂����炭Depth24bit�AStencil8bit�Ƃ����Ǝv��
		//���DUNORM��Unsigned NORMalized�̗��Ŏw�肳�ꂽ�͈͂�0.0�`1.0�ɂ���炵��
		texture2d_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;//�e�N�X�`���[�t�H�[�}�b�g
		//�}���`�T���v�����O�ݒ�
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;           //�e�N�X�`���̎g�p���@(�f�t�H���g)
		texture2d_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;  //Bind�ݒ��Depth,Stencil�ɐݒ�
		//���\�[�X�ւ�CPU�̃A�N�Z�X�����ɂ��Ă̐ݒ�
		//Read��Write�����邪�A�����ɂ��Č���͍l����K�v�͂Ȃ��͂��Ȃ̂ŁA
		//�f�t�H���g�l�ł���Ǝv����0�����Ă���
		//��read��write��enum(D3D11_CPU_ACCESS_FLAG)�ŗp�ӂ���Ă���
		texture2d_desc.CPUAccessFlags = 0;
		//���\�[�X�I�v�V�����̃t���O
		//Microsoft Docs�Ńt���O���m�F��������
		//�ʏ�g�p�Ńt���O��ݒ肷��K�v�͂Ȃ��Ǝv����̂�
		//0�Ƃ��Ă���
		texture2d_desc.MiscFlags = 0;
		//texture2d_desc�̏��Ńe�N�X�`�����쐬
		hr = device->CreateTexture2D(&texture2d_desc, NULL, &depth_stencil_buffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc{};
		depth_stencil_view_desc.Format = texture2d_desc.Format;// View�̃t�H�[�}�b�g(Texture�̂��̂��g�p)
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Texture2D.MipSlice = 0; //�ŏ��Ɏg�p����~�b�v�}�b�v�̃��x�����w��
		hr = device->CreateDepthStencilView(
			depth_stencil_buffer,            //DSV�Ƃ��Ďg�p�����Texture
			&depth_stencil_view_desc,        //DSV�̐ݒ�
			depth_stencil_view.GetAddressOf()//ID3D11DepthStencilView�̏o�͐�
		);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		depth_stencil_buffer->Release();
	}
	//�r���[�|�[�g�̐ݒ�
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
	//�T���v���[�X�e�[�g
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

		//�[�x�e�X�g:�I�� �[�x���C�g:�I��
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc{};
		depth_stencil_desc.DepthEnable = TRUE;                         //�[�x�e�X�g���s�����̃e�X�g.TRUE�Ńe�X�g������
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//�[�x�e�X�g���s���ۂ̌��f�[�^�Ə㏑������f�[�^���m�̔�r���@�̎w��
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //�[�x�f�[�^���������ލۂ̃}�X�N�̐ݒ�
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//�[�x�e�X�g:�I�� �[�x���C�g:�I�t
		depth_stencil_desc.DepthEnable = TRUE;                         //�[�x�e�X�g���s�����̃e�X�g.TRUE�Ńe�X�g������
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//�[�x�e�X�g���s���ۂ̌��f�[�^�Ə㏑������f�[�^���m�̔�r���@�̎w��
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //�[�x�f�[�^���������ލۂ̃}�X�N�̐ݒ�
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_ON_ZW_OFF)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//�[�x�e�X�g:�I�t �[�x���C�g:�I��
		depth_stencil_desc.DepthEnable = FALSE;                         //�[�x�e�X�g���s�����̃e�X�g.TRUE�Ńe�X�g������
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;//�[�x�e�X�g���s���ۂ̌��f�[�^�Ə㏑������f�[�^���m�̔�r���@�̎w��
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //�[�x�f�[�^���������ލۂ̃}�X�N�̐ݒ�
		hr = device->CreateDepthStencilState(&depth_stencil_desc, depth_stencil_states[static_cast<size_t>(DEPTH_STATE::ZT_OFF_ZW_ON)].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
		//�[�x�e�X�g:�I�t �[�x���C�g:�I�t
		depth_stencil_desc.DepthEnable = FALSE;                         //�[�x�e�X�g���s�����̃e�X�g.TRUE�Ńe�X�g������
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;//�[�x�e�X�g���s���ۂ̌��f�[�^�Ə㏑������f�[�^���m�̔�r���@�̎w��
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;    //�[�x�f�[�^���������ލۂ̃}�X�N�̐ݒ�
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

// �f�X�g���N�^
Graphics::~Graphics()
{
}

