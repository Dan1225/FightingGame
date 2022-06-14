#pragma once
#include <memory>
#include "character.h"
class Character;

enum CHARA_NUM
{
	PLAYER_1,
	PLAYER_2,

	MAX_PLAYER
};

enum  ALL_CHARACTERS
{
	chara_a,
	chara_b,
	chara_c,
};
class CharacterManager
{
private:
	//Character* chara[2];
public:
	Character* chara[MAX_PLAYER] = { nullptr };

	CharacterManager();
	~CharacterManager() {}
};