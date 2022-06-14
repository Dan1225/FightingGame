//*********************************************************
//
//		�u�����h�I�u�W�F�N�g
//
//*********************************************************
#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <assert.h>

class Blender
{
public:
    enum BLEND_STATE
    {
        BS_NONE,    //����
        BS_ALPHA,   //�L��
        BS_ADD,     //�ǉ�
        BS_SUBTRACT,//���Z
        BS_REPLACE, //����
        BS_MULTIPLY,//�|����
        BS_LIGHTEN, //���邭
        BS_DARKEN,  //�Â�
        BS_SCREEN,  //���
        BS_END      //�I��
    };
    Microsoft::WRL::ComPtr<ID3D11BlendState> states[BS_END];
    Blender(ID3D11Device* device)
    {
        HRESULT hr = S_OK;
        D3D11_BLEND_DESC blend_desc = {};
        // BS_NONE
        blend_desc.AlphaToCoverageEnable = FALSE;                                       //���̋@�\���g���Ƃ���true��ݒ肷��
        blend_desc.IndependentBlendEnable = FALSE;                                      //�����_�[�^�[�Q�b�g���ƂɈقȂ�u�����f�B���O���s�������߂�t���O false����RenderTarget[0]�̓��e�����ׂĎg�p����܂�
        blend_desc.RenderTarget[0].BlendEnable = FALSE;                                 //�u�����f�B���O��L���ɂ��邩�̃t���O
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;                          //�o�͐�̂��Ƃ��Ƃ������l
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;                        //�s�N�Z���V�F�[�_����̏o��
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                        //�s�N�Z���V�F�[�_����̏o�͂Əo�͐�̂��Ƃ��Ƃ������l��RGB���ǂ̂悤�ɑg�ݍ��킹�邩�̎w��
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;                     //SrcBlend��Alpha������
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;                   //BlendOp��Alpha������
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//RGBA���A�������݂��������̃r�b�g�𗧂Ă邱�Ƃŏ������ޒl�𐧌�ł���
        hr = device->CreateBlendState(&blend_desc, states[BS_NONE].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_ALPHA
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_ALPHA].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_ADD
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_ADD].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_SUBTRACT
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_SUBTRACT].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_REPLACE
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_REPLACE].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_MULTIPLY
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_MULTIPLY].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_LIGHTEN
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_LIGHTEN].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_DARKEN
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MIN;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MIN;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_DARKEN].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");

        // BS_SCREEN
        blend_desc.AlphaToCoverageEnable = FALSE;
        blend_desc.IndependentBlendEnable = FALSE;
        blend_desc.RenderTarget[0].BlendEnable = TRUE;
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        hr = device->CreateBlendState(&blend_desc, states[BS_SCREEN].GetAddressOf());
        if (FAILED(hr))assert("NONE_BLEND ERROR");
    }
};