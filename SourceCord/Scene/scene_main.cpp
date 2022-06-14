#include <iostream>

#include "../Scene/scene.h"
#include "../Graphics/framework.h"
#include "../header_file/collision.h"
#include "../header_file/key_input.h"

void Scene_Main::Render_Imgui()
{
	ImGui::Begin("Guard");
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Between"))
	{
		ImGui::SliderFloat("Between", &between, 0.0f, 1716.0f);

		ImGui::TreePop();
	}
	ImGui::End();
}

Scene_Main::Scene_Main()
{
	Framework& framework = Framework::Instance();
	ID3D11Device* device = framework.GetDevice();
	//�R���X�g���N�^
	Ready[0] = std::make_unique<Sprite>(device, L"Assets//fonts//font4.png");
	Ready[1] = std::make_unique<Sprite>(device, L"Assets//fonts//font4.png");
	back = std::make_unique<Sprite>(device, L"Assets//Scene_Main//Stage.png");
	//�L�����N�^�[
	Characters = std::make_unique<CharacterManager>();
	player_cirno = std::make_unique<Player_Cirno>(device);
	player_cirno2 = std::make_unique<Player_Cirno2>(device);
	//player_tei = std::make_unique<Player_Tei>(device);
	//UI
	gauge[0] = std::make_unique<UI>();
}


void Scene_Main::Initialize()
{
	Framework& framework = Framework::Instance();
	ID3D11Device* device = framework.GetDevice();

	Game_State = COUNT_DOWN;
	count_down = 30 * 3;
	//Init
	Characters->chara[PLAYER_1] = player_cirno.get();
	Characters->chara[PLAYER_2] = player_cirno2.get();

	for (int i = 0; i < CHARA_NUM::MAX_PLAYER; ++i)
	{
		if (Characters->chara[i] == nullptr)continue;
		Characters->chara[i]->Init();
	}
	gauge[0]->Init();
}

void Scene_Main::RoundInit()
{
	for (int i = 0; i < CHARA_NUM::MAX_PLAYER; ++i)
	{
		if (Characters->chara[i] == nullptr)continue;
		Characters->chara[i]->RoundInit();
	}
}

void Scene_Main::Update(float elapsedTime)
{
	//�����̗���
	switch (Game_State)
	{
	case COUNT_DOWN://Ready Fight!!�̕\���ƃJ�E���g�_�E��
		if(count_down == 0)
		{
			Game_State++;
		}
		count_down--;
		break;
	case BATTLE_START://�o�g���X�^�[�g
		for (int i = 0; i < CHARA_NUM::MAX_PLAYER; ++i)
		{
			if (Characters->chara[i] == nullptr)continue;
			Characters->chara[i]->Update(elapsedTime);

			if (player_cirno->GetCharaFrame().AttackFlg == true)
			{
				if (Hit(player_cirno->GetAttack(), player_cirno2->GetDamage(), 4.0f))//�L�����l����ł͂Ȃ�Characters[]�������Ă���
				{
					player_cirno2->Damage();
				}
				else
				{
					player_cirno->Flip = false;
				}
			}
		}
		Push(Characters->chara[PLAYER_1]->GetCollision(), Characters->chara[PLAYER_2]->GetCollision(), 4.0f);//�����o������

		if (KeyInput::KeyTrigger() & KEY_START)
		{
			Game_State++;
		}
		break;
	case ROUND_INIT://�ǂ��炩�����E���h��2����Ă��Ȃ������烉�E���h�X�V��
		RoundInit();
		Game_State = 0;
		count_down = 30 * 3;
		break;
	}
}

void Scene_Main::Render()
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

	FLOAT color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11RenderTargetView* rtv = framework.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = framework.GetDepthStencilView();
	context->ClearRenderTargetView(rtv, color);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->OMSetRenderTargets(1, &rtv, dsv);

	ID3D11BlendState* bs = framework.GetBlendState(Blender::BS_ALPHA);
	context->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	//�w�i�`��
	back->render(context, 0, 0, screenWidth, screenHeight, 0, 0, 1773, 896, 0, 1, 1, 1, 1.0);
	//Ready Fight!!�̕����`��
	if (count_down > 0)
	{
		Ready[0]->textout(context, "Ready", 800, 400, 64, 64, 1, 0, 0, 1);
	}
	else
	{
		Ready[1]->textout(context, "Fight", 800, 400, 64, 64, 1, 0, 0, 1);
	}
	//�L�����N�^�[�̕`��
	for (int i = 0; i < CHARA_NUM::MAX_PLAYER; ++i)
	{
		if (Characters->chara[i] == nullptr)continue;
		//player_tei->Update(elapsedTime);
		Characters->chara[i]->Draw(context);
		Characters->chara[i]->Render_ImGui();
	}
	//ImGui�`��
	Render_Imgui();
	//UI�̕`��
	gauge[0]->Draw();
}

void Scene_Main::Push(Push_Rect p1, Push_Rect p2, float Scale)
{
	//1P
	float l1 = p1.x;                 //��
	float r1 = p1.x + (p1.w * Scale);//�E
	float u1 = p1.y;                 //��
	float b1 = p1.y + (p1.h * Scale);//��
	//2P
	float l2 = p2.x;
	float r2 = p2.x + (p2.w * Scale);
	float u2 = p2.y;
	float b2 = p2.y + (p2.h * Scale);


	//�����o������
	if (r1 > l2)//1P��2P�̍����ɐG�ꂽ��
	{
		Move(Characters->chara[0], Characters->chara[1], Scale);
	}
	//if (l1 > r2)   return false;
	//if (b1 < u2)   return false;
	//if (u1 > b2)   return false;

	//return true;
}

void Scene_Main::Move(Character* p1, Character* p2, float Scale)
{
	p1->GetCollision().x = p2->GetCollision().x - p2->GetCollision().w * Scale;//�����o������p�̋�`�����炳�Ȃ��悤��
	p1->GetPos().x = p1->GetCollision().x - p1->GetCollision().w - player_cirno->right_offset;
	p2->GetPos().x += 10;      //�������Ă����瑊��������o��
	p2->GetCollision().x += 10;//�����o���p�̋�`��������

	//p1->lp.Attack_HitBox.x = p1->GetPos().x + 50 * Scale;//�U������̂���Ȃ��悤�ɂ����ōX�V
	//p1->mp.Attack_HitBox.x = p1->GetPos().x + p1->right_offset * Scale;
}

bool Scene_Main::Hit(Attack_Rect p1, Damage_Rect p2, float Scale)
{
	//1P
	float l1 = p1.x;                 //��
	float r1 = p1.x + p1.w;          //�E
	float u1 = p1.y;                 //��
	float b1 = p1.y + p1.h;          //��
	//2P
	float l2 = p2.x;
	float r2 = p2.x + (p2.w * Scale);
	float u2 = p2.y;
	float b2 = p2.y + (p2.h * Scale);

	if (r1 < l2)   return false;
	if (l1 > r2)   return false;
	if (b1 < u2)   return false;
	if (u1 > b2)   return false;
	return true;
}

bool Scene_Main::BattleEnd(int p1Win, int p2Win)
{
	if (p1Win == 2 || p2Win == 2)
	{
		return true;
	}

	return false;
}


bool Scene_Main::Flip(DirectX::XMFLOAT2 p1, DirectX::XMFLOAT2 p2)
{
	if (p1.x > p2.x)
	{
		return true;
	}
	return false;
}