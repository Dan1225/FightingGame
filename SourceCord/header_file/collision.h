#pragma once

#include <memory>

#include "../Graphics/sprite.h"
#include <DirectXMath.h>
#include <functional>

#define MAX_COLLIDERS 50
typedef struct Push_Rect
{
	float x, y;
	float w, h;
} Push_Rect;

typedef struct Attack_Rect
{
	float x, y;
	float w, h;
} Attack_Rect;

typedef struct Damage_Rect
{
	float x, y;
	float w, h;
} Damage_Rect;

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_PLAYER,
	COLLIDER_PLAYER2,
	COLLIDER_PLAYER_SHOT,
	COLLIDER_PLAYER2_SHOT,
	COLLIDER_PLAYER_HIT,
	COLLIDER_PLAYER2_HIT,

	COLLIDER_MAX
};