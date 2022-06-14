#pragma once
//個人的に追加したもの
#include <Windows.h>
#include <tchar.h>
#include <sstream>
#include <wrl.h>

#include "../header_file/misc.h"
#include "../header_file/high_resolution_timer.h"

#ifdef USE_IMGUI
#include "../header_file/Blender.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

#include<d3d11.h>
#include "sprite.h"
#include "sprite_batch.h"
#include "../header_file/primitive.h"
#include "../header_file/key_input.h"

constexpr long SCREEN_WIDTH{ 1920 };
constexpr long SCREEN_HEIGHT{ 1080 };
constexpr bool FULLSCREEN{ FALSE };
CONST LPCWSTR APPLICATION_NAME{ L"FightingGame" };

class Framework
{
public:
	static Framework* inst;
	CONST HWND hwnd = NULL;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];

	Microsoft::WRL::ComPtr<ID3D11PixelShader> replaced_pixel_shader;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> replaced_shader_resource_view;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizer_states[3];

	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection; //ビュー・プロジェクション変換行列
		DirectX::XMFLOAT4 light_direction;   //ライトの向き
		DirectX::XMFLOAT4 camera_position;   //カメラの位置
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	std::unique_ptr<Blender> blend;
	std::unique_ptr <Primitive> primitive;
	//std::unique_ptr<GamePad> gamePad;

	static Framework& Instance() { return *inst; }

	Framework(HWND hwnd);
	~Framework();

	Framework(const Framework&) = delete;
	Framework& operator=(const Framework&) = delete;
	Framework(Framework&&) noexcept = delete;
	Framework& operator=(Framework&&) noexcept = delete;

	std::unique_ptr<Sprite> font;

	//ゲッター
	ID3D11Device* GetDevice() const { return device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return device_context.Get(); }
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }
	ID3D11BlendState* GetBlendState(Blender::BLEND_STATE blend_state) const { return blend->states[blend_state].Get(); }
	Sprite* GetFont() const { return font.get(); }
	UINT GetScreenWidth() const { return SCREEN_WIDTH; }
	UINT GetScreenHeight() const { return SCREEN_HEIGHT; }
	float GetElapsedTime() const { return elapsed_time; }

	int run()
	{
		MSG msg{};

		if (!initialize())
		{
			return 0;
		}

#ifdef USE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), device_context.Get());
		ImGui::StyleColorsDark();
#endif

		while (WM_QUIT != msg.message)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				tictoc.tick();
				calculate_frame_stats();
				update(tictoc.time_interval());
				render(tictoc.time_interval());
			}
		}

#ifdef USE_IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
#endif
#if 1
		BOOL fullscreen;
		swap_chain->GetFullscreenState(&fullscreen, 0);
		if (fullscreen)
		{
			swap_chain->SetFullscreenState(FALSE, 0);
		}
#endif

		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
	}

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
#ifdef USE_IMGUI
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
#endif
		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
		}
		break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE)
			{
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
			break;
		case WM_ENTERSIZEMOVE:
			tictoc.stop();
			break;
		case WM_EXITSIZEMOVE:
			tictoc.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize();
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	high_resolution_timer tictoc;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	void calculate_frame_stats()
	{
		if (++frames, (tictoc.time_stamp() - elapsed_time) >= 1.0f)
		{
			float fps = static_cast<float>(frames);

			std::wostringstream outs;
			outs.precision(6);
			outs << APPLICATION_NAME << L" : FPS : " << fps << L" / " << L"Frame Time : " << 1000.0f / fps << L" (ms)";
			SetWindowTextW(hwnd, outs.str().c_str());

			frames = 0;
			elapsed_time += 1.0f;
		}
	}
};

