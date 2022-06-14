#include "../Player/player_cirno.h"
#include "../Graphics/framework.h"
#include "../header_file/key_input.h"

void Player_Cirno::Render_ImGui()
{
	ImGui::Begin("1P");
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Param"))
	{
		ImGui::SliderFloat("Cirno_Pos.x", &Pos.x, 0.0f, 1800.0f);
		ImGui::SliderFloat("Cirno_Pos.y", &Pos.y, 0.0f, 606.0f);
		ImGui::SliderFloat("Size.x", &Size.x, 0.0f, 100.0f);
		ImGui::SliderFloat("Size.y", &Size.y, 0.0f, 100.0f);

		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Push_Collision"))
	{
		ImGui::SliderFloat("Push_Collision.x", &Push_Col.x, 0.0f, 1716.0f);
		ImGui::SliderFloat("Push_Collision.y", &Push_Col.y, 0.0f, 606.0f);
		ImGui::SliderFloat("Push_Collision.w", &Push_Col.w, 0.0f, 80.0f);
		ImGui::SliderFloat("Push_Collision.h", &Push_Col.h, 0.0f, 96.0f);

		ImGui::SliderFloat("HitBox.x", &lp.Attack_HitBox.x, 0.0f, 1716.0f);
		ImGui::SliderFloat("HitBox.y", &lp.Attack_HitBox.y, 0.0f, 606.10f);
		ImGui::SliderFloat("HitBox.w", &lp.Attack_HitBox.w, 0.0f, 80.0f);
		ImGui::SliderFloat("HitBox.h", &lp.Attack_HitBox.h, 0.0f, 96.0f);

		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Debug"))
	{
		ImGui::Checkbox("Push_Rect", &Push_Debug);
		ImGui::Checkbox("Flip", &Flip);

		ImGui::Text("Win: %d",Win);

		ImGui::TreePop();
	}
	ImGui::End();
}

Player_Cirno::Player_Cirno(ID3D11Device* device)
{
	texture = std::make_shared<Sprite>(device, L"Assets//Player_Cirno//player.png");
	primitive = std::make_unique<Sprite>(device, L"Assets//Scene_Main//Rect.png");

	Angle = 0;            //��]
	Scale = 0;            //�g��̔{��
	Gravity = 0.0f;       //�v���C���[�ɂ�����d��
	JumpPower = 0.0f;     //�W�����v�̋���
	Ground = 0.0f;        //�n�ʂɂȂ���W�̒l(y�̂�)
	Hp = 0;               //�v���C���[�̗̑�
	Win = 0;              //�v���C���[�̏�����(�ő�2���E���h)
	Pos = { 0,0 };        //�v���C���[�̍��W
	sPos = { 0,0 };       //�e�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	Size = { 0,0 };       //�v���C���[�̑傫��(��{�I ���F80,�c�F96)
	Anime_rect = { 0,0,0,0 };   //�A�j���[�V�����؂蔲����`
	Velocity = { 0,0 };   //�L�����N�^�[�̃X�s�[�h
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { 0,0,0,0 };

	Push_Debug = false;
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = false;

	cirno_states current_state = Cirno_WAIT1;
	Current_animation = &Idle1;
}


void Player_Cirno::Init()
{
	Angle = 0;            //��]
	Scale = 4.0f;         //�g��̔{��
	Gravity = 0.6f;       //�v���C���[�ɂ�����d��
	JumpPower = 20.0f;    //�W�����v�̐���
	Hp = 10;              //�v���C���[�̗̑�
	Win = 0;              //�v���C���[�̏�����(�ő�2���E���h)
	Pos = { 500,600 };    //�v���C���[�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	Size = { 80,96 };     //�v���C���[�̑傫��(��{�I ���F80,�c�F96)
	Velocity = { 10, 10 };//�L�����N�^�[�̃X�s�[�h
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { Pos.x + left_offset * Scale,Pos.y + 10 * Scale,51,84 };//�����o������p�̋�`(�摜�̌��_����̂̑傫���ɍ��킹�Ă��炵�Ă���)

	Rect_Color = { 1.0,0.0,0.0,0.5 };

	Push_Debug = false;       //��`�\�����邩�ǂ���
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = false;

	State = Cirno_BUTTON_INIT;
	cirno_states current_state = Cirno_ST_UNKNOWN;
	Current_animation = &Idle1;

	//�A�j���[�V�����ݒ�
	const int Idle1Colliders = 1;//�����蔻���BOX�̍ő吔
	Rect IdleHitBox[Idle1Colliders] = { {0,0,80,96} };//�����蔻��̈ʒu�Ƒ傫���ݒ�
	COLLIDER_TYPE IdleColliderType[Idle1Colliders] = { {COLLIDER_PLAYER} }; //�����蔻��̑���
	//�ҋ@���1
	Idle1.PushBack({ 0,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 80,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 160,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 240,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 320,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 400,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 480,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 560,0, 80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	//�O�i
	const int ForwardColliders = 1;
	Rect ForwardHitBox[ForwardColliders] = { {0,0,80,96} };
	COLLIDER_TYPE ForwardColliderType[ForwardColliders] = { {COLLIDER_PLAYER} };
	Forward.PushBack({ 0,  480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 80, 480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 160,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 240,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 320,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 400,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 480,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 560,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 640,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 720,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 800,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 880,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	Forward.PushBack({ 960,480, 80,96 }, 6, { 29,6 }, ForwardColliders, ForwardHitBox, ForwardColliderType);
	//���
	const int BackwardColliders = 1;
	Rect BackwardHitBox[BackwardColliders] = { {0,0,80,96} };
	COLLIDER_TYPE BackwardColliderType[BackwardColliders] = { {COLLIDER_PLAYER} };
	Backward.PushBack({ 0,    576, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
	Backward.PushBack({ 80,   576, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
	Backward.PushBack({ 160,  576, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
	Backward.PushBack({ 240,  576, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
	Backward.PushBack({ 320,  576, 80,96 }, 6, { 29,6 }, BackwardColliders, BackwardHitBox, BackwardColliderType);
	//�����オ��
	const int StandingColliders = 1;
	Rect StandingHitBox[StandingColliders] = { {0,0,80,96} };
	COLLIDER_TYPE StandingColliderType[StandingColliders] = { {COLLIDER_PLAYER} };
	standing.PushBack({ 0,    384, 80,96 }, 6, { 29,6 }, StandingColliders, StandingHitBox, StandingColliderType);
	standing.PushBack({ 80,   384, 80,96 }, 6, { 29,6 }, StandingColliders, StandingHitBox, StandingColliderType);
	//���Ⴊ�ݓr��
	const int CrouchingColliders = 1;
	Rect CrouchingHitBox[CrouchingColliders] = { {0,0,80,96} };
	COLLIDER_TYPE CrouchingColliderType[CrouchingColliders] = { {COLLIDER_PLAYER} };
	crouching.PushBack({ 0,    384, 80,96 }, 6, { 29,6 }, CrouchingColliders, CrouchingHitBox, CrouchingColliderType);
	crouching.PushBack({ 80,   384, 80,96 }, 6, { 29,6 }, CrouchingColliders, CrouchingHitBox, CrouchingColliderType);
	//���Ⴊ�ݒ�
	const int CrouchColliders = 1;
	Rect CrouchHitBox[CrouchColliders] = { 0,0,80,96 };
	COLLIDER_TYPE CrouchColliderType[CrouchColliders] = { {COLLIDER_PLAYER} };
	crouch.PushBack({ 160,384, 80,96 }, 1, { 29,6 }, CrouchColliders, CrouchHitBox, CrouchColliderType);
	//�W�����v
	const int JumpColliders = 1;
	Rect JumpHitBox[JumpColliders] = { {0,0,80,96} };
	COLLIDER_TYPE JumpColliderType[JumpColliders] = { {COLLIDER_PLAYER} };
	JumpAnim.PushBack({ 0,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 80,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 160,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 240,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 320,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 400,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 480,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 560,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 640,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 720,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);
	JumpAnim.PushBack({ 800,288,80,96 }, 6, { 29,96 }, JumpColliders, JumpHitBox, JumpColliderType);

	//��P
	const int LpColliders = 1;
	Rect LpHitBox[LpColliders] = { 0,0,80,96 };
	COLLIDER_TYPE LpColliderType[LpColliders] = { {COLLIDER_PLAYER} };
	lp.Attack_HitBox = { Pos.x + 50 * Scale,Pos.y + 39 * Scale ,30 * Scale,20 * Scale };//�U������̈ʒu�A�傫���̐ݒ�
	lp.PushBack({ 0, 1344,80,96 }, 4, lp.Attack_HitBox, false);
	lp.PushBack({ 80, 1344,80,96 }, 4, lp.Attack_HitBox, false);
	lp.PushBack({ 160,1344,80,96 }, 4, lp.Attack_HitBox, true);
	lp.PushBack({ 240,1344,80,96 }, 4, lp.Attack_HitBox, true);
	lp.PushBack({ 320,1344,80,96 }, 4, lp.Attack_HitBox, true);
	lp.PushBack({ 400,1344,80,96 }, 4, lp.Attack_HitBox, false);

	//��P
	const int MpColliders = 1;
	Rect MpHitBox[MpColliders] = { 0,0,128,96 };
	COLLIDER_TYPE MpColliderType[MpColliders] = { { COLLIDER_PLAYER } };
	mp.Attack_HitBox = { Pos.x + 53 * Scale,Pos.y + 39 * Scale ,33 * Scale,20 * Scale };//�U������̈ʒu�A�傫���̐ݒ�
	mp.PushBack({ 0,   1728,80,96 }, 6, mp.Attack_HitBox, false);
	mp.PushBack({ 128, 1728,80,96 }, 6, mp.Attack_HitBox, false);
	mp.PushBack({ 256, 1728,80,96 }, 6, mp.Attack_HitBox, false);
	mp.PushBack({ 384, 1728,80,96 }, 6, mp.Attack_HitBox, true);
	mp.PushBack({ 512, 1728,80,96 }, 6, mp.Attack_HitBox, true);
	mp.PushBack({ 640, 1728,80,96 }, 6, mp.Attack_HitBox, true);
	mp.PushBack({ 768, 1728,80,96 }, 6, mp.Attack_HitBox, true);
	mp.PushBack({ 896, 1728,80,96 }, 6, mp.Attack_HitBox, true);
	mp.PushBack({ 1024,1728,80,96 }, 6, mp.Attack_HitBox, false);
	mp.PushBack({ 1152,1728,80,96 }, 6, mp.Attack_HitBox, false);

	//�_���[�W(��)
	low_damage.PushBack({ 0,864,80,96 }, 6);
	low_damage.PushBack({ 80,864,80,96 }, 6);
	low_damage.PushBack({ 160,864,80,96 }, 6);

	round_emote.PushBack({ 0,1632,80,96 }, 6);
	round_emote.PushBack({ 80,1632,80,96 }, 6);
	round_emote.PushBack({ 160,1632,80,96 }, 6);
	round_emote.PushBack({ 240,1632,80,96 }, 6);
	round_emote.PushBack({ 320,1632,80,96 }, 6);
	round_emote.PushBack({ 400,1632,80,96 }, 6);
	round_emote.PushBack({ 480,1632,80,96 }, 6);
	round_emote.PushBack({ 560,1632,80,96 }, 6);
	round_emote.PushBack({ 640,1632,80,96 }, 6);
	round_emote.PushBack({ 720,1632,80,96 }, 6);
	round_emote.PushBack({ 800,1632,80,96 }, 6);
	round_emote.PushBack({ 880,1632,80,96 }, 6);

}

void Player_Cirno::RoundInit()
{
	Hp = 10;              //�v���C���[�̗̑�
	Pos = { 500,600 };    //�v���C���[�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { Pos.x + left_offset * Scale,Pos.y + 10 * Scale,51,84 };//�����o������p�̋�`(�摜�̌��_����̂̑傫���ɍ��킹�Ă��炵�Ă���)

	Rect_Color = { 1.0,0.0,0.0,0.5 };

	Push_Debug = false;       //��`�\�����邩�ǂ���
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = false;

	State = Cirno_BUTTON_INIT;
	cirno_states current_state = Cirno_ST_UNKNOWN;
	Current_animation = &Idle1;
}


void Player_Cirno::Update(float elapsedTime)
{
	Cirno_Frame = Current_animation->GetCurrentFrame();

	Action();//�s��

	if (KeyInput::KeyTrigger() & KEY_M)
	{
		Win += 1;
	}

	Jump(JumpPower); //�W�����v

	Wall(); //�ǂƂ̔���
}

void Player_Cirno::Draw(ID3D11DeviceContext* context)
{
	Framework& framework = Framework::Instance();

	ID3D11BlendState* bs = framework.GetBlendState(Blender::BS_ALPHA);
	context->OMSetBlendState(bs, nullptr, 0xFFFFFFFF);

	int HitBoxesQnt = Cirno_Frame.GetColliderQnt();

	for (int i = 0; i < HitBoxesQnt; i++)
	{
		Anime_rect = Cirno_Frame.hitboxrects[i];
	}

	Anime_rect = Cirno_Frame.frame;

	texture->render(context,
		Pos.x, Pos.y,
		Size.x * Scale, Size.y * Scale,
		//Size.x, Size.y,
		Anime_rect,
		Angle,
		1, 1, 1, 1,
		Flip);

	Attack();
	//���オ���_
	//Pos.x                                             ����
	//Pos.x + (Size.x * Scale)                          �E��
	//Pos.y + (Size.y * Scale)                          ����
	//Pos.x + (Size.x * Scale), Pos.y + (Size.y * Scale)�E��
	//Pos.x + Size.x, Pos.y + Size.y

	Debug_Draw(context);
}

void Player_Cirno::Action()
{
	switch (State)
	{
	case Cirno_BUTTON_INIT:
		if (KeyInput::KeyState() & KEY_D && JumpFlg == false) { State = Cirno_FORWARD_MOVE; }
		else if (KeyInput::KeyState() & KEY_A && JumpFlg == false) { State = Cirno_BACKWARD_MOVE; }
		else if (KeyInput::KeyState() & KEY_S) { State = Cirno_CROUCH; }
		else if (KeyInput::KeyTrigger() & KEY_U && AttackFlg == false) { State = Cirno_STANDING_LIGHT; }
		else if (KeyInput::KeyTrigger() & KEY_I && AttackFlg == false) { State = Cirno_STANDING_MID; }
		else if (KeyInput::KeyTrigger() & KEY_Z) { State = Cirno_DAMAGE1; } //�摜���]�e�X�g
		else { State = Cirno_WAIT1; }
		break;
	case Cirno_WAIT1:
		Current_animation = &Idle1;
		State = Cirno_BUTTON_INIT;
		break;
	case Cirno_FORWARD_MOVE:
		Current_animation = &Forward;

		Pos.x += Velocity.x;
		Push_Col.x += Velocity.x;
		lp.Attack_HitBox.x += Velocity.x;
		mp.Attack_HitBox.x += Velocity.x;

		if (KeyInput::KeyRelease() & KEY_D)
		{
			State = Cirno_BUTTON_INIT;
		}
		break;
	case Cirno_BACKWARD_MOVE:
		Current_animation = &Backward;

		Pos.x -= Velocity.x;
		Push_Col.x -= Velocity.x;
		lp.Attack_HitBox.x -= Velocity.x;
		mp.Attack_HitBox.x -= Velocity.x;

		if (KeyInput::KeyRelease() & KEY_A)
		{
			State = Cirno_BUTTON_INIT;
		}
		break;
	case Cirno_CROUCH:
		Current_animation = &crouch;
		if (KeyInput::KeyRelease() & KEY_S)
		{
			State = Cirno_BUTTON_INIT;
		}
		break;
	case Cirno_STANDING_LIGHT:
		Current_animation = &lp;
		if (Anime_rect.x >= 400)
		{
			AttackFlg = false;
			lp.ResetAnimation();
			lp.loop = false;
			State = Cirno_BUTTON_INIT;
		}
		break;
	case Cirno_STANDING_MID:
		Current_animation = &mp;
		if (Anime_rect.x >= 1152)//���̃R�}���ɂȂ�����A�j���[�V���������Z�b�g����
		{
			AttackFlg = false;
			mp.ResetAnimation();
			mp.loop = false;
			State = Cirno_BUTTON_INIT;
		}
		break;
	}
}

void Player_Cirno::Jump(float JumpPower)
{
	if (KeyInput::KeyTrigger() & KEY_W && JumpPos.y == 0 && JumpFlg == false)
	{
		JumpPos.y = -JumpPower;              //�����x
		JumpFlg = true;
	}
	if (JumpFlg == true)
	{
		Current_animation = &JumpAnim;
	}
	Pos.y = Pos.y + JumpPos.y;        //�t���[�����ɑ��x�����Z
	Push_Col.y = Push_Col.y + JumpPos.y;
	JumpPos.y = JumpPos.y + Gravity;  //�t���[�����ɉ����x�𑬓x�ɉ��Z
	if (Pos.y >= 600)                 //Y���W����ʒu��艺�ɂ���΁A�n�ʂɏՓ˂������ƂɂȂ�
	{
		JumpPos.y = 0;
		JumpFlg = false;
	}
}

void Player_Cirno::Attack()
{
	Framework& framework = Framework::Instance();
	ID3D11DeviceContext* context = framework.GetDeviceContext();
	if (Cirno_Frame.AttackFlg == true)
	{
		if (State == Cirno_STANDING_LIGHT)
		{
			primitive->render(context,
				lp.Attack_HitBox.x, lp.Attack_HitBox.y,
				lp.Attack_HitBox.w, lp.Attack_HitBox.h,
				0, 0,
				1200, 448,
				0,
				0, 1, 0, 0.5);
		}
		if (State == Cirno_STANDING_MID)
		{
			primitive->render(context,
				mp.Attack_HitBox.x, mp.Attack_HitBox.y,
				mp.Attack_HitBox.w, mp.Attack_HitBox.h,
				0, 0,
				1200, 448,
				0,
				0, 1, 0, 0.5);
		}
	}
}

Attack_Rect& Player_Cirno::GetAttack()
{
	if (Cirno_Frame.AttackFlg == true)
	{
		if (State == Cirno_STANDING_LIGHT)
		{
			return lp.Attack_HitBox;
		}
		if (State == Cirno_STANDING_MID)
		{
			return mp.Attack_HitBox;
		}
	}
}

void Player_Cirno::Damage()
{

}

void Player_Cirno::Debug_Draw(ID3D11DeviceContext* context)
{

	if (Push_Debug == true)
	{
		primitive->render(context,
			Push_Col.x, Push_Col.y,
			Push_Col.w * Scale, Push_Col.h * Scale,
			0, 0,
			1200, 448,
			0,
			Rect_Color
		);
		/*primitive->render(context,
			Pos.x,Pos.y,
			Size.x * Scale,Size.y * Scale,
			Anime_rect,
			Angle,
			0,0,1,0.5
		);*/
	}
}