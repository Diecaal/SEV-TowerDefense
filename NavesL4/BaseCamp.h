#pragma once

#include "Projectile.h" 
#include "Actor.h"
#include "ShootingAction.h"
#include <cmath>
#include <list>

class BaseCamp : public Actor
{
public:
	BaseCamp(float x, float y, Game* game);
	ShootingAction* shootingAction;
	void loseLife();
	const int maxLifes = 6;
	int lifes = 6;
};
