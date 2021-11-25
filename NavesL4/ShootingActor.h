#pragma once

#include "Actor.h";
#include "Projectile.h";
#include "Enemy.h"
#include <cmath>
#include <list>

class ShootingActor : public Actor
{
public:
	ShootingActor(string fileName, float x, float y, Game* game);
	Projectile* shoot(list<Enemy*> enemies);
	bool enemyInRange(list<Enemy*> enemies);
	bool shootAvailable();
	int shootingCooldown = 0;
	float shootingRange = 160.0;
	struct vectorShoot
	{
		float x = -1;
		float y = -1;
	};
	struct vectorShoot shootingVector;
};

