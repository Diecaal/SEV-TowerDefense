#pragma once

#include "Actor.h";
#include "Projectile.h";
#include "BaseEnemy.h"
#include <cmath>
#include "Audio.h"
#include <list>

class ShootingAction
{
public:
	ShootingAction(Actor* currentActor, Game* game);
	Game* game;
	Actor* currentActor;
	Audio* audioShoot;
	Projectile* shoot(list<BaseEnemy*> enemies);
	bool enemyInRange(list<BaseEnemy*> enemies);
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

