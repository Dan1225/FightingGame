#pragma once

constexpr auto MAX_FRAMES = 60;
#include "collision.h"
#include "frame.h"

class Animation
{
public:
	bool loop = true;
	bool AttackFlg = false;
	Attack_Rect Attack_HitBox = { 0,0,0,0};
	Frame frames[MAX_FRAMES];

private:
	int frames_passed = 0;
	int current_frame;
	int last_frame = 0;
	int loops = 0;

public:
	void PushBack(const Rect& rect,const int maxFrames,Attack_Rect attack_rect,bool Attack)
	{
		frames[last_frame].frame = rect;
		frames[last_frame].MaxFrames = maxFrames;
		frames[last_frame].Attack = attack_rect;
		frames[last_frame].AttackFlg = Attack;

		last_frame++;
	}

	void PushBack(const Rect& rect, const int maxFrames, DirectX::XMINT2 pivotPosition,int nColliders, Rect hitbox[],COLLIDER_TYPE type[])
	{
		frames[last_frame].frame = rect;
		frames[last_frame].MaxFrames = maxFrames;
		frames[last_frame].pivotPosition = pivotPosition;

		for(int i = 0; i < nColliders; i++)
		{
			frames[last_frame].AddColliderToFrame(hitbox[i], type[i]);
		}

		last_frame++;
	}

	void PushBack(const Rect& rect, const int maxFrames, DirectX::XMINT2 pivotPosition, int nColliders, Rect hitbox[], COLLIDER_TYPE type[],bool Attack)
	{
		frames[last_frame].frame = rect;
		frames[last_frame].MaxFrames = maxFrames;
		frames[last_frame].pivotPosition = pivotPosition;
		frames[last_frame].AttackFlg = Attack;

		for (int i = 0; i < nColliders; i++)
		{
			frames[last_frame].AddColliderToFrame(hitbox[i], type[i]);
		}

		last_frame++;
	}
	void PushBack(const Rect& rect, const int maxFrames)
	{
		frames[last_frame].frame = rect;
		frames[last_frame].MaxFrames = maxFrames;

		last_frame++;
	}
	Frame& GetCurrentFrame() //現在のフレームを取得
	{
		if(frames_passed < frames[current_frame].MaxFrames)//フレーム数が最大値になるまで更新する
		{
			frames_passed++;
		}
		else//フレームを最初に戻す
		{
			frames_passed = 0;
			current_frame++;
		}

		if(current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[current_frame];
	}

	Rect& GetCurrentFrameBox()
	{
		return GetCurrentFrame().frame;
	}

	Rect& GetHitBox()
	{
		return frames[current_frame].Hitbox;
	}

	bool Finished() const
	{
		return loops > 0;
	}

	void ResetAnimation()
	{
		frames_passed = 0;
		current_frame = 0;
	}
};
