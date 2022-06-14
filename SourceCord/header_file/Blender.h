//*********************************************************
//
//		ブレンドオブジェクト
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
        BS_NONE,    //無効
        BS_ALPHA,   //有効
        BS_ADD,     //追加
        BS_SUBTRACT,//減算
        BS_REPLACE, //交換
        BS_MULTIPLY,//掛ける
        BS_LIGHTEN, //明るく
        BS_DARKEN,  //暗く
        BS_SCREEN,  //画面
        BS_END      //終了
    };
    Microsoft::WRL::ComPtr<ID3D11BlendState> states[BS_END];
    Blender(ID3D11Device* device)
    {
        HRESULT hr = S_OK;
        D3D11_BLEND_DESC blend_desc = {};
        // BS_NONE
        blend_desc.AlphaToCoverageEnable = FALSE;                                       //この機能を使うときはtrueを設定する
        blend_desc.IndependentBlendEnable = FALSE;                                      //レンダーターゲットごとに異なるブレンディングを行うか決めるフラグ falseだとRenderTarget[0]の内容がすべて使用されます
        blend_desc.RenderTarget[0].BlendEnable = FALSE;                                 //ブレンディングを有効にするかのフラグ
        blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;                          //出力先のもともとあった値
        blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;                        //ピクセルシェーダからの出力
        blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;                        //ピクセルシェーダからの出力と出力先のもともとあった値のRGBをどのように組み合わせるかの指定
        blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;                     //SrcBlendのAlpha成分版
        blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;                   //BlendOpのAlpha成分版
        blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;//RGBA内、書き込みたい部分のビットを立てることで書き込む値を制御できる
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