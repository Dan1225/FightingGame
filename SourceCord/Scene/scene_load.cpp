#include <thread>
#include "../Scene/scene.h"
#include "../Graphics/framework.h"
void Scene_Load::Initialize()
{
	//スレッド開始
	//※この関数が抜けてもLoadingThread()を実行させるため手放す
	std::thread thread(LoadingThread, this);
	thread.detach();
	load = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//Scene_Main//Rect.png");
	font2 = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//fonts//font4.png");
	cirno_emote = std::make_unique<Sprite>(Framework::Instance().GetDevice(), L"Assets//Player_Cirno//cirno_emote.png");

	current_animation = &Activate1;
	//発動(カードあり)アニメーション
	Activate1.PushBack({ 0,   0,   80,96 }, 8);
	Activate1.PushBack({ 80,  0,   80,96 }, 8);
	Activate1.PushBack({ 160, 0,   80,96 }, 8);
	Activate1.PushBack({ 240, 0,   80,96 }, 8);
	Activate1.PushBack({ 320, 0,   80,96 }, 8);
	Activate1.PushBack({ 400, 0,   80,96 }, 8);
	Activate1.PushBack({ 480, 0,   80,96 }, 8);
	Activate1.PushBack({ 560, 0,   80,96 }, 8);
	Activate1.PushBack({ 640, 0,   80,96 }, 8);
	Activate1.PushBack({ 720, 0,   80,96 }, 8);
	Activate1.PushBack({ 800, 0,   80,96 }, 8);
	Activate1.PushBack({ 880, 0,   80,96 }, 8);
	Activate1.PushBack({ 960, 0,   80,96 }, 8);
	Activate1.PushBack({ 1040,0,   80,96 }, 8);
	Activate1.PushBack({ 1120,0,   80,96 }, 8);
	Activate1.PushBack({ 1200,0,   80,96 }, 8);
	Activate1.PushBack({ 1280,0,   80,96 }, 8);
	//発動(カードなし)アニメーション
	Activate2.PushBack({ 0,     96,  80,96 }, 8);
	Activate2.PushBack({ 80,    96,  80,96 }, 8);
	Activate2.PushBack({ 160,   96,  80,96 }, 8);
	Activate2.PushBack({ 240,   96,  80,96 }, 8);
	Activate2.PushBack({ 320,   96,  80,96 }, 8);
	Activate2.PushBack({ 400,   96,  80,96 }, 8);
	Activate2.PushBack({ 480,   96,  80,96 }, 8);
	Activate2.PushBack({ 560,   96,  80,96 }, 8);
	Activate2.PushBack({ 640,   96,  80,96 }, 8);
	//笑いアニメーション
	Laugh.PushBack({ 0,   192 ,80,96 }, 8);
	Laugh.PushBack({ 80,  192 ,80,96 }, 8);
	Laugh.PushBack({ 160, 192 ,80,96 }, 8);
	Laugh.PushBack({ 240, 192 ,80,96 }, 8);
	Laugh.PushBack({ 320, 192 ,80,96 }, 8);
	Laugh.PushBack({ 400, 192 ,80,96 }, 8);
	Laugh.PushBack({ 480, 192 ,80,96 }, 8);
	Laugh.PushBack({ 560, 192 ,80,96 }, 8);
	Laugh.PushBack({ 640, 192 ,80,96 }, 8);
	Laugh.PushBack({ 720, 192 ,80,96 }, 8);
	Laugh.PushBack({ 800, 192 ,80,96 }, 8);
	Laugh.PushBack({ 880, 192 ,80,96 }, 8);
	Laugh.PushBack({ 960, 192 ,80,96 }, 8);
	Laugh.PushBack({ 1040,192 ,80,96 }, 8);
	Laugh.PushBack({ 1120,192 ,80,96 }, 8);
	//発動アニメーション3
	Activate3.PushBack({ 0,  288,80,96 }, 8);
	Activate3.PushBack({ 80, 288,80,96 }, 8);
	Activate3.PushBack({ 160,288,80,96 }, 8);
	Activate3.PushBack({ 240,288,80,96 }, 8);
	Activate3.PushBack({ 320,288,80,96 }, 8);
	Activate3.PushBack({ 400,288,80,96 }, 8);
	Activate3.PushBack({ 480,288,80,96 }, 8);
	Activate3.PushBack({ 560,288,80,96 }, 8);
	Activate3.PushBack({ 640,288,80,96 }, 8);
	Activate3.PushBack({ 720,288,80,96 }, 8);
	//勝利ポーズ1
	Win1.PushBack({ 0,  384,80,96 }, 8);
	Win1.PushBack({ 80, 384,80,96 }, 8);
	Win1.PushBack({ 160,384,80,96 }, 8);
	Win1.PushBack({ 240,384,80,96 }, 8);
	Win1.PushBack({ 320,384,80,96 }, 8);
	Win1.PushBack({ 400,384,80,96 }, 8);
	Win1.PushBack({ 480,384,80,96 }, 8);
	Win1.PushBack({ 560,384,80,96 }, 8);
	Win1.PushBack({ 640,384,80,96 }, 8);
	Win1.PushBack({ 720,384,80,96 }, 8);
	Win1.PushBack({ 800,384,80,96 }, 8);
	Win1.PushBack({ 880,384,80,96 }, 8);
	Win1.PushBack({ 960,384,80,96 }, 8);
	//勝利ポーズ2

	//泣き
	Cry.PushBack({ 0,    576,80,96 }, 8);
	Cry.PushBack({ 80,   576,80,96 }, 8);
	Cry.PushBack({ 160,  576,80,96 }, 8);
	Cry.PushBack({ 240,  576,80,96 }, 8);
	Cry.PushBack({ 320,  576,80,96 }, 8);
	Cry.PushBack({ 400,  576,80,96 }, 8);
	Cry.PushBack({ 480,  576,80,96 }, 8);
	Cry.PushBack({ 560,  576,80,96 }, 8);
	Cry.PushBack({ 640,  576,80,96 }, 8);
	Cry.PushBack({ 720,  576,80,96 }, 8);
	//おびえる
	Frightened.PushBack({ 0,  672,80,96 }, 8);
	Frightened.PushBack({ 80, 672,80,96 }, 8);
	srand((unsigned int)time(NULL));
	rnd_state = rand() % 7 + 1;
}

void Scene_Load::Update(float elapsedTime)
{
	//次のシーンへ
	if (nextScene->initialized)
	{
		SceneManager::Instance().ChangeScene(nextScene.release());
		return;
	}
	//ローディング演出
	timer -= elapsedTime;
	if (timer < 0)
	{
		timer = 0.1f;

		strcpy_s(message, "Now Loading");
		counter = (counter + 1) % 3;
		for (int i = 0; i < counter; ++i)
		{
			strcat_s(message, ".");
		}
	}
	//ローディングアニメーション
	frame = current_animation->GetCurrentFrame();
	switch(rnd_state)
	{
	case 1:
		current_animation = &Activate1;
		rnd_state = 0;
		break;
	case 2:
		current_animation = &Activate2;
		rnd_state = 0;
		break;
	case 3:
		current_animation = &Laugh;
		rnd_state = 0;
		break;
	case 4:
		current_animation = &Activate3;
		rnd_state = 0;
		break;
	case 5:
		current_animation = &Win1;
		rnd_state = 0;
		break;
	case 6:
		current_animation = &Cry;
		rnd_state = 0;
		break;
	case 7:
		current_animation = &Frightened;
		rnd_state = 0;
		break;
	}
}

void Scene_Load::Render()
{
	//DeviceContextの取得と画面クリア
	Framework& framework = Framework::Instance();
	ID3D11DeviceContext* context = framework.GetDeviceContext();
	Rect r;
	int HitBoxesQnt = frame.GetColliderQnt();

	for (int i = 0; i < HitBoxesQnt; i++)
	{
		r = frame.hitboxrects[i];
	}
	r = frame.frame;
	//Now Loading…描画
	load->render(context, 0, 0, 1920, 1080, 0, 0, 1920, 1920, 0, 0, 0, 0, 1);
	font2->textout(context, message, 1500, 1000, 16, 16, 1, 1, 1, 1);
	cirno_emote->render(context,
		1680, 860,
		80 * 2, 96 * 2,//画像のスケール
		r,
		0,
		1.0f, 1.0f, 1.0f, 1.0f);
}

void Scene_Load::LoadingThread(Scene_Load* scene)
{
	Sleep(4000);
	//
	scene->nextScene->Initialize();
	scene->nextScene->initialized = true;
}