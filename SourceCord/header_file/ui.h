#pragma once
#include <memory>
#include "../Graphics/sprite.h"

class UI
{
private:
	std::unique_ptr<Sprite> Gauge[1];
	std::unique_ptr<Sprite> Time;
public:

	void Init();
	void Update();
	void Draw();
};