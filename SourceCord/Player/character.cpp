#include "../Player/character.h"
#include "../Graphics/framework.h"

Character::Character()
{
	Angle = 0;            //回転
	Scale = 0;            //拡大の倍率
	Gravity = 0.0f;       //プレイヤーにかかる重力
	JumpPower = 0.0f;     //ジャンプの強さ
	Ground = 0.0f;        //地面になる座標の値(yのみ)
	Hp = 0;               //プレイヤーの体力
	Win = 0;              //プレイヤーの勝利数(最大2ラウンド)
	Pos = { 0,0 };        //プレイヤーの座標
	sPos = { 0,0 };       //弾の座標
	JumpPos = { 0,0 };    //プレイヤーの座標(ジャンプ中)
	Size = { 0,0 };       //プレイヤーの大きさ(基本的 横：80,縦：96)
	Anime_rect = { 0,0,0,0 };   //アニメーション切り抜き矩形
	Velocity = { 0,0 };   //キャラクターのスピード
	AttackFlg = false;    //攻撃しているかどうか
	JumpFlg = false;      //ジャンプしているかどうか
	CrouchingFlg = false; //しゃがんでいるかどうか

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
	//画面行き過ぎないように
	if (SCREEN_WIDTH - Push_Col.w * Scale < Push_Col.x)
	{//右画面端
		Pos.x = Push_Col.x + Push_Col.w - right_offset * Scale - 77;
		Push_Col.x = SCREEN_WIDTH - Push_Col.w * Scale;
	}
	if (0 > Push_Col.x)
	{//左画面端
		Pos.x = -left_offset * Scale;
		Push_Col.x = 0;
	}
	if (SCREEN_HEIGHT - Size.y * Scale - 90 < Pos.y)
	{//地面との判定
		Pos.y = SCREEN_HEIGHT - Size.y * Scale - 96; //下画面端(54は地面にしたい座標と下画面端の差分)
		Push_Col.y = SCREEN_HEIGHT - Push_Col.h * Scale - 104;
	}
	if (0 > Pos.y)
	{//上画面端
		Pos.y = 0;
	}
}

void Character::Jump(float JumpPower)
{
	if (KeyInput::KeyTrigger() & KEY_W && JumpPos.y == 0 && JumpFlg == false)
	{
		JumpPos.y -= JumpPower;              //初速度
		JumpFlg = true;
	}

	Pos.y = Pos.y + JumpPos.y;        //フレーム毎に速度を加算
	Push_Col.y = Push_Col.y + JumpPos.y;
	JumpPos.y = JumpPos.y + Gravity;  //フレーム毎に加速度を速度に加算
	if (Pos.y >= 600)                 //Y座標が基準位置より下にあれば、地面に衝突したことになる
	{
		JumpPos.y = 0;
		JumpFlg = false;
	}
}