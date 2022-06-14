#include "../Player/player_cirno2.h"
#include "../Graphics/framework.h"
//#include "../header_file/key_input.h"

void Player_Cirno2::Render_ImGui()
{
	ImGui::Begin("2P");
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Param"))
	{
		ImGui::SliderFloat("Cirno2_Pos.x", &Pos.x, 0.0f, 1600.0f);
		ImGui::SliderFloat("Cirno2_Pos.y", &Pos.y, 0.0f, 606.0f);

		ImGui::Checkbox("Push_Rect", &Push_Debug);
		ImGui::Checkbox("hurt_box", &HitBox_Debug);
		ImGui::Checkbox("Flip", &Flip);
		ImGui::TreePop();
	}
	ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Once);
	if (ImGui::TreeNode("Collision"))
	{
		ImGui::SliderFloat("CollisionPos.x", &Pos.x, 0.0f, 1600.0f);
		ImGui::SliderFloat("CollisionPos.y", &Pos.y, 0.0f, 606.0f);
		ImGui::SliderFloat("Collisionsize.x", &Size.x, 0.0f, 80.0f);
		ImGui::SliderFloat("Collisionsize.y", &Size.y, 0.0f, 96.0f);

		ImGui::TreePop();
	}
	ImGui::End();
}

Player_Cirno2::Player_Cirno2(ID3D11Device* device)
{
	texture = std::make_unique<Sprite>(device, L"Assets//Player_Cirno//player2.png");
	primitive = std::make_unique<Sprite>(device, L"Assets//Scene_Main//Rect.png");
	//Shot = std::make_unique<Sprite>(device, L"Assets//Player_Cirno//shot.png");

	Angle = 0;            //��]
	Scale = 0;            //�g��̔{��
	Gravity = 0.0f;       //�v���C���[�ɂ�����d��
	JumpPower = 0.0f;     //�W�����v�̋���
	Ground = 0.0f;        //�n�ʂɂȂ���W�̒l(y�̂�)
	Hp = 0;               //�v���C���[�̗̑�
	Win = 0;              //�v���C���[�̏�����(�ő�2���E���h)
	Pos = { 0,0 };        //�v���C���[�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	Size = { 0,0 };       //�v���C���[�̑傫��(��{�I ���F80,�c�F96)
	Anime_rect = { 0,0,0,0 };   //�A�j���[�V�����؂蔲����`
	Velocity = { 0,0 };   //�L�����N�^�[�̃X�s�[�h
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { 0,0,0,0 };
	hurt_box = { 0,0,0,0 };

	Push_Debug = false;
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = false;

	cirno2_states current_state = Cirno2_ST_UNKNOWN;
	Current_animation = &Idle1;
}


void Player_Cirno2::Init()
{
	Angle = 0;            //��]
	Scale = 4.0f;         //�g��̔{��
	Gravity = 0.6f;       //�v���C���[�ɂ�����d��
	JumpPower = 20.0f;    //
	Hp = 10;              //�v���C���[�̗̑�
	Win = 0;              //�v���C���[�̏�����(�ő�2���E���h)
	Pos = { 1200,600 };   //�v���C���[�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	Size = { 80,96 };     //�v���C���[�̑傫��(��{�I ���F80,�c�F96)
	Velocity = { 10, 10 };//�L�����N�^�[�̃X�s�[�h
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { Pos.x + 17 * Scale,Pos.y + 10 * Scale,51,84 };//�����o������p�̋�`(�摜�̌��_����̂̑傫���ɍ��킹�Ă��炵�Ă���)
	hurt_box = { Pos.x + 17 * Scale,Pos.y + 10 * Scale,51,84 };//���ꔻ��̐ݒ�

	Rect_Color = { 0.0,0.0,1.0,0.5 };

	Push_Debug = false;
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = true;

	State = Cirno2_BUTTON_INIT;
	cirno2_states current_state = Cirno2_ST_UNKNOWN;
	Current_animation = &Idle1;

	//�A�j���[�V�����ݒ�
	const int Idle1Colliders = 1;//�����蔻���BOX�̍ő吔
	Rect IdleHitBox[Idle1Colliders] = { {0,0,80,96} };//�����蔻��̈ʒu�Ƒ傫���ݒ�
	COLLIDER_TYPE IdleColliderType[Idle1Colliders] = { {COLLIDER_PLAYER} }; //�����蔻��̑���
	//�ҋ@���1
	Idle1.PushBack({ 0,0,   80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
	Idle1.PushBack({ 80,0,  80,96 }, 8, { 25,10 }, Idle1Colliders, IdleHitBox, IdleColliderType);
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
	//��P
	const int LpColliders = 1;
	Rect LpHitBox[LpColliders] = { 0,0,80,96 };
	COLLIDER_TYPE LpColliderType[LpColliders] = { {COLLIDER_PLAYER} };
	lp.PushBack({ 0, 1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);
	lp.PushBack({ 80,1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);
	lp.PushBack({ 160,1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);
	lp.PushBack({ 240,1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);
	lp.PushBack({ 320,1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);
	lp.PushBack({ 400,1344,80,96 }, 4, { 30,20 }, LpColliders, LpHitBox, LpColliderType);

	//��P
	const int MpColliders = 1;
	Rect MpHitBox[MpColliders] = { 0,0,128,96 };
	COLLIDER_TYPE MpColliderType[MpColliders] = { { COLLIDER_PLAYER } };
	mp.PushBack({ 0,   1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 128, 1728,80,96 }, 6, { 26,20 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 256, 1728,80,96 }, 6, { 25,18 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 384, 1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 512, 1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 640, 1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 768, 1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 896, 1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 1024,1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);
	mp.PushBack({ 1152,1728,80,96 }, 6, { 37,25 }, MpColliders, MpHitBox, MpColliderType);

	//�_���[�W(��)
	low_damage.PushBack({   0,864,80,96 }, 1);
	low_damage.PushBack({  80,864,80,96 }, 1);
	low_damage.PushBack({ 160,864,80,96 }, 1);
}

void Player_Cirno2::RoundInit()
{
	Hp = 10;              //�v���C���[�̗̑�
	Pos = { 1200,600 };   //�v���C���[�̍��W
	JumpPos = { 0,0 };    //�v���C���[�̍��W(�W�����v��)
	AttackFlg = false;    //�U�����Ă��邩�ǂ���
	JumpFlg = false;      //�W�����v���Ă��邩�ǂ���
	CrouchingFlg = false; //���Ⴊ��ł��邩�ǂ���

	Push_Col = { Pos.x + 17 * Scale,Pos.y + 10 * Scale,51,84 };//�����o������p�̋�`(�摜�̌��_����̂̑傫���ɍ��킹�Ă��炵�Ă���)
	hurt_box = { Pos.x + 17 * Scale,Pos.y + 10 * Scale,51,84 };//���ꔻ��̐ݒ�

	Rect_Color = { 0.0,0.0,1.0,0.5 };

	Push_Debug = false;
	Attack_Debug = false;
	HitBox_Debug = false;
	Flip = true;

	State = Cirno2_BUTTON_INIT;
	cirno2_states current_state = Cirno2_ST_UNKNOWN;
	Current_animation = &Idle1;
}


void Player_Cirno2::Update(float elapsedTime)
{
	Cirno_Frame = Current_animation->GetCurrentFrame();

	Action();//�s��

	//Jump(); //�W�����v

	Wall(); //�ǂƂ̔���
}
void Player_Cirno2::Draw(ID3D11DeviceContext* context)
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
	if (Cirno_Frame.AttackFlg == true)
	{

	}

	//���オ���_
	//Pos.x                                             ����
	//Pos.x + (Size.x * Scale)                          �E��
	//Pos.y + (Size.y * Scale)                          ����
	//Pos.x + (Size.x * Scale), Pos.y + (Size.y * Scale)�E��
	//Pos.x + Size.x, Pos.y + Size.y

	texture->render(context,
		Pos.x, Pos.y,
		Size.x * Scale, Size.y * Scale,
		Anime_rect,
		Angle,
		1, 1, 1, 1,
		Flip);

	Debug_Draw(context);
}

void Player_Cirno2::Jump(float JumpPower)
{
	if (KeyInput::KeyTrigger() & KEY_W && JumpPos.y == 0 && JumpFlg == false)
	{
		JumpPos.y = -JumpPower;       //�����x
		JumpFlg = true;
	}
	Pos.y = Pos.y + JumpPos.y;        //�t���[�����ɑ��x�����Z
	JumpPos.y = JumpPos.y + Gravity;  //�t���[�����ɉ����x�𑬓x�ɉ��Z

	if (Pos.y >= 600)                 //Y���W����ʒu��艺�ɂ���΁A�n�ʂɏՓ˂������ƂɂȂ�
	{
		JumpPos.y = 0;
		JumpFlg = false;
	}
}

void Player_Cirno2::Action()
{
	//switch (State)
	//{
	//case Cirno_BUTTON_INIT:
	//	if (KeyInput::KeyState() & KEY_D && JumpFlg == false) { State = Cirno_FORWARD_MOVE; }
	//	else if (KeyInput::KeyState() & KEY_A && JumpFlg == false) { State = Cirno_BACKWARD_MOVE; }
	//	else if (KeyInput::KeyState() & KEY_S) { State = Cirno_CROUCH; }
	//	else if (KeyInput::KeyTrigger() & KEY_U && AttackFlg == false) { State = Cirno_STANDING_LIGHT; }
	//	else if (KeyInput::KeyTrigger() & KEY_I && AttackFlg == false) { State = Cirno_STANDING_MID; }
	//	else if (KeyInput::KeyTrigger() & KEY_Z) { Flip = !Flip; } //�摜���]�e�X�g
	//	else { State = Cirno_WAIT1; }
	//	break;
	//case Cirno_WAIT1:
	//	Current_animation = &Idle1;
	//	State = Cirno_BUTTON_INIT;
	//	break;
	//case Cirno_FORWARD_MOVE:
	//	Current_animation = &Forward;
	//	Pos.x += Velocity.x;
	//	Push_Col.x += Velocity.x;
	//	if (KeyInput::KeyRelease() & KEY_D)
	//	{
	//		State = Cirno_BUTTON_INIT;
	//	}
	//	break;
	//case Cirno_BACKWARD_MOVE:
	//	Current_animation = &Backward;
	//	Pos.x -= Velocity.x;
	//	Push_Col.x -= Velocity.x;
	//	if (KeyInput::KeyRelease() & KEY_A)
	//	{
	//		State = Cirno_BUTTON_INIT;
	//	}
	//	break;
	//case Cirno_CROUCH:
	//	Current_animation = &crouch;
	//	if (KeyInput::KeyRelease() & KEY_S)
	//	{
	//		State = Cirno_BUTTON_INIT;
	//	}
	//	break;
	//case Cirno_STANDING_LIGHT:
	//	Current_animation = &lp;
	//	AttackFlg = true;
	//	if (AttackFlg == true)
	//	{
	//		if (Anime_rect.x >= 400)
	//		{
	//			AttackFlg = false;
	//			lp.ResetAnimation();
	//			lp.loop = false;
	//			State = Cirno_BUTTON_INIT;
	//		}
	//	}
	//	break;
	//case Cirno_STANDING_MID:
	//	Current_animation = &mp;
	//	AttackFlg = true;
	//	if (AttackFlg == true)
	//	{
	//		if (Anime_rect.x >= 1152)
	//		{
	//			AttackFlg = false;
	//			mp.ResetAnimation();
	//			mp.loop = false;
	//			State = Cirno_BUTTON_INIT;
	//		}
	//	}
	//	break;
	//}
}

void Player_Cirno2::Attack()
{

}

void Player_Cirno2::Damage()
{
	Current_animation = &low_damage;
	low_damage.ResetAnimation();
	low_damage.loop = false;
	State = Cirno2_BUTTON_INIT;
}

void Player_Cirno2::Debug_Draw(ID3D11DeviceContext* context)
{
	if (Push_Debug == true)
	{
		//�����o���������
		primitive->render(context,
			Push_Col.x, Push_Col.y,
			Push_Col.w * Scale, Push_Col.h * Scale,
			0, 0,
			1200, 448,
			0,
			1, 0, 0, 0.5
		);
	}
	if (HitBox_Debug == true)
	{
		//���ꔻ�����
		primitive->render(context,
			hurt_box.x, hurt_box.y,
			hurt_box.w * Scale, hurt_box.h * Scale,
			0, 0,
			1200, 448,
			0,
			Rect_Color
		);
	}
}