#include "../Scene/scene.h"
#include "../Graphics/framework.h"
#include "../header_file/key_input.h"

Scene_ModeSelect::Scene_ModeSelect()
{
	//gamePad = std::make_unique<GamePad>();
	Mode[0] = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//Scene_Main//Rect.png");
	Mode[1] = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//fonts//font4.png");
}


void Scene_ModeSelect::Initialize()
{
	Mode_State = VS_MODE;
}

void Scene_ModeSelect::Update(float elapsedTime)
{
	//gamePad->Update();
}

void Scene_ModeSelect::Render()
{
	Framework& framework = Framework::Instance();
	ID3D11DeviceContext* context = framework.GetDeviceContext();
	Mode[0]->render(context, 0, 0, 1920, 1080, 0, 0, 1920, 1920, 0, 0, 0, 0, 1);
	switch (Mode_State)
	{
	case VS_MODE:
		Mode[1]->textout(context, "1P VS 2P",800, 200, 64, 64, 1, 1, 1, 1);
		Mode[1]->textout(context, "Practice",800, 400, 32, 32, 1, 1, 1, 1);
		Mode[1]->textout(context, "Title",   800, 600, 32, 32, 1, 1, 1, 1);

		if (KeyInput::KeyRelease() & KEY_S)
		{
			Mode_State++;
		}

		if (KeyInput::KeyRelease() & KEY_START)
		{
			SceneManager::Instance().ChangeScene(new Scene_Load(new Scene_Main()));
		}
		break;
	case PRACTICE_MODE:
		Mode[1]->textout(context, "1P VS 2P", 800, 200, 32, 32, 1, 1, 1, 1);
		Mode[1]->textout(context, "Practice", 800, 400, 64, 64, 1, 1, 1, 1);
		Mode[1]->textout(context, "Title",    800, 600, 32, 32, 1, 1, 1, 1);

		if (KeyInput::KeyRelease() & KEY_W)
		{
			Mode_State--;
		}

		if (KeyInput::KeyRelease() & KEY_S)
		{
			Mode_State++;
		}

		if (KeyInput::KeyRelease() & KEY_START)
		{
			SceneManager::Instance().ChangeScene(new Scene_Load(new Scene_ModeSelect()));
		}
		break;
	case TITLE_BACK:
		Mode[1]->textout(context, "1P VS 2P",800, 200, 32, 32, 1, 1, 1, 1);
		Mode[1]->textout(context, "Practice",800, 400, 32, 32, 1, 1, 1, 1);
		Mode[1]->textout(context, "Title",   800, 600, 64, 64, 1, 1, 1, 1);

		if (KeyInput::KeyRelease() & KEY_W)
		{
			Mode_State--;
		}

		if (KeyInput::KeyRelease() & KEY_START)
		{
			SceneManager::Instance().ChangeScene(new Scene_Load(new Scene_Title()));
		}
		break;
	}
}

