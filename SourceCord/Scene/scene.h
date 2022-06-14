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
	//SceneLoad��SceneManager�����ɐݒ�ł���t���O
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
	std::unique_ptr<Scene>  nextScene;  //�V�[���؂�ւ��ϐ�
	std::unique_ptr<Sprite> cirno_emote;//���[�h�A�j���[�V�����p�̉摜
	std::unique_ptr<Sprite> load;       //���[�h���̔w�i
	std::unique_ptr<Sprite> font2;      //NowLoading�̕���
	std::unique_ptr<GamePad> gamePad;   //�R���g���[���[�̏�����(���݂͂܂�������)
	//Now Loading�c�A�j���[�V�����p
	float timer = 0.0f;
	int counter = 0;
	char message[32] = { 0 };
	//���[�h��ʃA�j���[�V����
	Animation* current_animation;
	Animation  Activate1,Activate2,Activate3,Laugh;
	Animation  Win1, Win2;
	Animation  Cry, Frightened;

	Frame frame;
	int rnd_state;//�A�j���[�V�����������_���őI�Ԃ��߂̕ϐ�
public:
	Scene_Load(Scene* nextScene)
	{   //���[�f�B���O��ɑJ�ڂ���V�[����ێ�
		this->nextScene.reset(nextScene);
	}
	virtual ~Scene_Load() {};
	//�p������
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
		TITLE_PUSH,//�{�^����t
		FADE_OUT,  //�t�F�[�h�A�E�g���s
	};
public:
	//�p������
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
	//�p������
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

	std::unique_ptr<Sprite>           Ready[2];     //�����\��
	std::unique_ptr<Sprite>           back;         //�w�i
	std::unique_ptr<Player_Cirno>     player_cirno; //�L����1
	std::unique_ptr<Player_Cirno2>    player_cirno2;//�L����2
	std::unique_ptr<CharacterManager> Characters;   //�}�l�[�W���[�ϐ�
	std::unique_ptr<UI>               gauge[1];     //�̗̓Q�[�W
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
	//�p������
	void Initialize() override;
	void Update(float elapsedTime) override;
	void Render() override;
	//�V�����ǉ������֐�
	void RoundInit();
	void Push(Push_Rect p1, Push_Rect p2, float Scale);
	bool Flip(DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2);
	void Move(Character* p1, Character* p2, float Scale);
	static bool Hit(Attack_Rect p1, Damage_Rect p2, float Scale);
	static bool BattleEnd(int p1Win, int p2Win);

	void Render_Imgui();
};

//----------------------------------------------------
//		SceneManager(�Ǘ�)�N���X
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
