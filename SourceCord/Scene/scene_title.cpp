#include "../Scene//scene.h"
#include "../Graphics/framework.h"
#include "../header_file/key_input.h"

void Scene_Title::ImGui_SceneChanger()
{
	ImGui::Begin("Scene_Change");      // Create a window called "Hello, world!" and append into it.
	if (ImGui::Button("Scene_Title"))
	{
		SceneManager::Instance().ChangeScene(new Scene_Title());
	}
	if (ImGui::Button("Scene_ModeSelect"))
	{
		SceneManager::Instance().ChangeScene(new Scene_ModeSelect());
	}
	if (ImGui::Button(u8"Scene_Main"))
	{
		SceneManager::Instance().ChangeScene(new Scene_Main());
	}
	ImGui::End();
}


void Scene_Title::Initialize()
{
	img = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//Title.png");
	font2 = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//fonts//font4.png");
	fadeout = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//Scene_Main//Rect.png");
	fade_alpha = 0.0f;
	Title_State = TITLE_PUSH;
	strcpy_s(message, "START SPACE");
}

void Scene_Title::Update(float elapsedTime)
{
	switch (Title_State)
	{
	case TITLE_PUSH:
		if (KeyInput::KeyTrigger() & KEY_START)
		{
			Title_State++;
		}
		break;
	case FADE_OUT:
		fade_alpha += 0.0167f;
		if (fade_alpha >= 1.5f)
		{
			SceneManager::Instance().ChangeScene(new Scene_Load(new Scene_ModeSelect()));
		}
		break;
	}
}

void Scene_Title::Render()
{

	Framework& framework = Framework::Instance();
	float screenWidth = static_cast<FLOAT>(framework.GetScreenWidth());
	float screenHeight = static_cast<FLOAT>(framework.GetScreenHeight());

	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	ID3D11DeviceContext* context = framework.GetDeviceContext();

	context->RSSetViewports(1, &viewport);

	FLOAT color[] = { 0.4f, 0.4f, 0.9f, 1.0f };
	ID3D11RenderTargetView* rtv = framework.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = framework.GetDepthStencilView();
	context->ClearRenderTargetView(rtv, color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	ID3D11BlendState* bs = framework.GetBlendState(Blender::BS_ALPHA);
	context->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	img->render(context, 0, 0, 1920, 1080, 0, 0, 1920, 1080, 0, 1, 1, 1, 1.0);
	font2->textout(context, message, 720, 900, 50, 50, 1, 1, 1, 1);
	if (fade_alpha > 0.0f)
	{
		fadeout->render(context, 0, 0, 1920, 1080, 0, 0, 1920, 1080, 0, 0, 0, 0, fade_alpha);
	}
	ImGui_SceneChanger();
}