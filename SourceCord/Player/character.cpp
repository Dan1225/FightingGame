#include "../Player/character.h"
#include "../Graphics/framework.h"

Character::Character()
{
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
	hurt_box = { 0,0,0,0 };
	Rect_Color = { 0,0,0,0 };

	Flip = false;
	Push_Debug = false;
	Attack_Debug = false;
	HitBox_Debug = false;
}


void Character::Wall()
{
	//��ʍs���߂��Ȃ��悤��
	if (SCREEN_WIDTH - Push_Col.w * Scale < Push_Col.x)
	{//�E��ʒ[
		Pos.x = Push_Col.x + Push_Col.w - right_offset * Scale - 77;
		Push_Col.x = SCREEN_WIDTH - Push_Col.w * Scale;
	}
	if (0 > Push_Col.x)
	{//����ʒ[
		Pos.x = -left_offset * Scale;
		Push_Col.x = 0;
	}
	if (SCREEN_HEIGHT - Size.y * Scale - 90 < Pos.y)
	{//�n�ʂƂ̔���
		Pos.y = SCREEN_HEIGHT - Size.y * Scale - 96; //����ʒ[(54�͒n�ʂɂ��������W�Ɖ���ʒ[�̍���)
		Push_Col.y = SCREEN_HEIGHT - Push_Col.h * Scale - 104;
	}
	if (0 > Pos.y)
	{//���ʒ[
		Pos.y = 0;
	}
}

void Character::Jump(float JumpPower)
{
	if (KeyInput::KeyTrigger() & KEY_W && JumpPos.y == 0 && JumpFlg == false)
	{
		JumpPos.y -= JumpPower;              //�����x
		JumpFlg = true;
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