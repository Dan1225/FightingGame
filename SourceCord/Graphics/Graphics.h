#pragma once
#include <memory>
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>

#ifdef USE_IMGUI
#include "../header_file/Blender.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern ImWchar glyphRangesJapanese[];
#endif

class Graphics
{
public:
	Graphics(HWND hWnd);
	~Graphics();

	// �C���X�^���X�擾
	static Graphics& Instance() { return *instance; }

	// �f�o�C�X�擾
	ID3D11Device* GetDevice() const { return device.Get(); }

	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() const { return device_context.Get(); }

	// �X���b�v�`�F�[���擾
	IDXGISwapChain* GetSwapChain() const { return swap_chain.Get(); }

	// �����_�[�^�[�Q�b�g�r���[�擾
	ID3D11RenderTargetView* GetRenderTargetView() const { return render_target_view.Get(); }

	// �f�v�X�X�e���V���r���[�擾
	ID3D11DepthStencilView* GetDepthStencilView() const { return depth_stencil_view.Get(); }

	// �V�F�[�_�[�擾
	//Shader* GetShader() const { return shader.get(); }

	// �X�N���[�����擾
	float GetScreenWidth() const { return screenWidth; }

	// �X�N���[�������擾
	float GetScreenHeight() const { return screenHeight; }

	// �f�o�b�O�����_���擾
	//DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }

	// ���C�������_���擾
	//LineRenderer* GetLineRenderer() const { return lineRenderer.get(); }

	// ImGui�����_���擾
	//ImGuiRenderer* GetImGuiRenderer() const { return imguiRenderer.get(); }

//	int run()
//	{
//		MSG msg{};
//
//		if (!initialize())
//		{
//			return 0;
//		}
//
//#ifdef USE_IMGUI
//		IMGUI_CHECKVERSION();
//		ImGui::CreateContext();
//		ImGui::GetIO().Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 14.0f, nullptr, glyphRangesJapanese);
//		ImGui_ImplWin32_Init(hwnd);
//		ImGui_ImplDX11_Init(device.Get(), device_context.Get());
//		ImGui::StyleColorsDark();
//#endif
//
//		while (WM_QUIT != msg.message)
//		{
//			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//			{
//				TranslateMessage(&msg);
//				DispatchMessage(&msg);
//			}
//			else
//			{
//				tictoc.tick();
//				calculate_frame_stats();
//				update(tictoc.time_interval());
//				render(tictoc.time_interval());
//			}
//		}
//
//#ifdef USE_IMGUI
//		ImGui_ImplDX11_Shutdown();
//		ImGui_ImplWin32_Shutdown();
//		ImGui::DestroyContext();
//#endif
//#if 1
//		BOOL fullscreen;
//		swap_chain->GetFullscreenState(&fullscreen, 0);
//		if (fullscreen)
//		{
//			swap_chain->SetFullscreenState(FALSE, 0);
//		}
//#endif
//
//		return uninitialize() ? static_cast<int>(msg.wParam) : 0;
//	}
//
//	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
//	{
//#ifdef USE_IMGUI
//		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) { return true; }
//#endif
//		switch (msg)
//		{
//		case WM_PAINT:
//		{
//			PAINTSTRUCT ps{};
//			BeginPaint(hwnd, &ps);
//
//			EndPaint(hwnd, &ps);
//		}
//		break;
//
//		case WM_DESTROY:
//			PostQuitMessage(0);
//			break;
//		case WM_CREATE:
//			break;
//		case WM_KEYDOWN:
//			if (wparam == VK_ESCAPE)
//			{
//				PostMessage(hwnd, WM_CLOSE, 0, 0);
//			}
//			break;
//		case WM_ENTERSIZEMOVE:
//			tictoc.stop();
//			break;
//		case WM_EXITSIZEMOVE:
//			tictoc.start();
//			break;
//		default:
//			return DefWindowProc(hwnd, msg, wparam, lparam);
//		}
//		return 0;
//	}

private:
	static Graphics* instance;

	Microsoft::WRL::ComPtr<ID3D11Texture2D>	depthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> device_context;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap_chain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> render_target_view;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depth_stencil_view;

	enum class SAMPLER_STATE { POINT, LINEAR, ANISOTROPIC };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler_states[3];

	enum class DEPTH_STATE { ZT_ON_ZW_ON, ZT_ON_ZW_OFF, ZT_OFF_ZW_ON, ZT_OFF_ZW_OFF };
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_states[4];

	struct scene_constants
	{
		DirectX::XMFLOAT4X4 view_projection; //�r���[�E�v���W�F�N�V�����ϊ��s��
		DirectX::XMFLOAT4 light_direction;   //���C�g�̌���
		DirectX::XMFLOAT4 camera_position;   //�J�����̈ʒu
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> constant_buffers[8];

	//std::unique_ptr<Shader>							shader;
	//std::unique_ptr<DebugRenderer>					debugRenderer;
	//std::unique_ptr<LineRenderer>					    lineRenderer;
	//std::unique_ptr<ImGuiRenderer>					imguiRenderer;

	float	screenWidth;
	float	screenHeight;
	CONST BOOL FULLSCREEN{ FALSE };
};