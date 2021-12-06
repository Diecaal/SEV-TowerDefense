#pragma once

#include "Actor.h"

class Projectile : public Actor
{
public:
	Projectile(float x, float y, Game* game);
	Projectile(float x, float y, Game* game, bool playerShoot);
	Projectile(float x, float y, Game* game, float vx, float vy);
	bool canDealDamage = true;
};

