#pragma once

#include "../Graphics/sprite.h"

constexpr auto MAX_COLLIDERS_PER_FRAME = 10;

enum COLLIDER_TYPE;

class Frame
{
public:
	Rect frame; //切り取るアニメーションの矩形
	int MaxFrames;
	DirectX::XMINT2 pivotPosition;
	Rect hitboxrects[MAX_COLLIDERS_PER_FRAME];
	COLLIDER_TYPE types[MAX_COLLIDERS_PER_FRAME];
	bool AttackFlg;
	Rect Hitbox;
	Attack_Rect Attack;
private:
	int last_collider;

public:
	void AddColliderToFrame(Rect rect, COLLIDER_TYPE type)
	{
		hitboxrects[last_collider] = rect;
		types[last_collider] = type;
		last_collider++;
	}

	int GetColliderQnt()
	{
		return  last_collider;
	}
};