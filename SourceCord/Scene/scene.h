#pragma once
#include <memory>
#include "../Graphics/sprite.h"
#include "../header_file/animation.h"
#include "../header_file/ui.h"
#include "../header_file/key_input.h"
#include "../Player/player_cirno.h"
#include "../Player/player_cirno2.h"

class Character;

class Scene
{
private:
	//SceneLoadとSceneManagerだけに設定できるフラグ
	friend class Scene_Load;
	friend class SceneManager;
	bool initialized = false;
public:
	Scene() {}
	virtual ~Scene() {}
	virtual void Initialize() = 0;
	virtual void Update(float elapsedTime) = 0;
	virtual void Render() = 0;
};


class Scene_Load : public Scene
{
private:
	std::unique_ptr<Scene>  nextScene;  //シーン切り替え変数
	std::unique_ptr<Sprite> cirno_emote;//ロードアニメーション用の画像
	std::unique_ptr<Sprite> load;       //ロード中の背景
	std::unique_ptr<Sprite> font2;      //NowLoadingの文字
	std::unique_ptr<GamePad> gamePad;   //コントローラーの初期化(現在はまだ未実装)
	//Now Loading…アニメーション用
	float timer = 0.0f;
	int counter = 0;
	char message[32] = { 0 };
	//ロード画面アニメーション
	Animation* current_animation;
	Animation  Activate1,Activate2,Activate3,Laugh;
	Animation  Win1, Win2;
	Animation  Cry, Frightened;

	Frame frame;
	int rnd_state;//アニメーションをランダムで選ぶための変数
public:
	Scene_Load(Scene* nextScene)
	{   //ローディング後に遷移するシーンを保持
		this->nextScene.reset(nextScene);
	}
	virtual ~Scene_Load() {};
	//継承部分
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
private:
	static void LoadingThread(Scene_Load* scene);
};

class Scene_Title :public Scene
{
private:
	std::unique_ptr<Sprite> img;
	std::unique_ptr<Sprite> font2;
	std::unique_ptr<Sprite> fadeout;
	std::unique_ptr<GamePad> gamePad;

	int Title_State = 0;
	float fade_alpha = 0.0f;
	char message[32] = { 0 };
	enum SCENE_STATE
	{
		TITLE_PUSH,//ボタン受付
		FADE_OUT,  //フェードアウト実行
	};
public:
	//継承部分
	Scene_Title() {}
	virtual ~Scene_Title() {}
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	//ImGui
	void ImGui_SceneChanger();
};

class Scene_ModeSelect : public Scene
{
private:
	std::unique_ptr<Sprite> Mode[3];
	std::unique_ptr<GamePad> gamePad;
	int Mode_State = 0;
	enum MODE_SELECT
	{
		VS_MODE,
		PRACTICE_MODE,
		TITLE_BACK,
	};
public:
	//継承部分
	Scene_ModeSelect();
	virtual ~Scene_ModeSelect() {}
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
};

class Scene_Main : public Scene
{
private:
	int Game_State;
	int count_down;
	int game_timer;

	std::unique_ptr<Sprite>           Ready[2];     //文字表示
	std::unique_ptr<Sprite>           back;         //背景
	std::unique_ptr<Player_Cirno>     player_cirno; //キャラ1
	std::unique_ptr<Player_Cirno2>    player_cirno2;//キャラ2
	std::unique_ptr<CharacterManager> Characters;   //マネージャー変数
	std::unique_ptr<UI>               gauge[1];     //体力ゲージ
	enum ROUND
	{
		COUNT_DOWN,
		BATTLE_START,
		ROUND_INIT,
	};
	bool flg;
	float between;
public:
	Scene_Main();
	virtual ~Scene_Main() {}
	//継承部分
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	//新しく追加した関数
	void RoundInit();
	void Push(Push_Rect p1, Push_Rect p2, float Scale);
	bool Flip(DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2);
	void Move(Character* p1, Character* p2, float Scale);
	static bool Hit(Attack_Rect p1, Damage_Rect p2, float Scale);
	static bool BattleEnd(int p1Win, int p2Win);

	void Render_Imgui();
};

//----------------------------------------------------
//		SceneManager(管理)クラス
//----------------------------------------------------
class SceneManager
{
private:
	std::unique_ptr<Scene> currentScene;

private:
	SceneManager() {}
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager inst;
		return inst;
	}

	void Update(float elapsedTime);
	void Render();
	void ChangeScene(Scene* new_scene);

};
