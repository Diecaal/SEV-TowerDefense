#pragma once

#include "BaseEnemy.h"
#include "Animation.h"

class Enemy : public BaseEnemy
{
public:
	Enemy(float x, float y, Game* game);
	void update();
};
