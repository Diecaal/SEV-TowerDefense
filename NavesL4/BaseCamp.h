#pragma once

#include "ShootingActor.h"
#include "Projectile.h" 
#include <cmath>
#include <list>

class BaseCamp : public ShootingActor
{
public:
	BaseCamp(float x, float y, Game* game);
	void looseLife();
	int lifes = 3;
};
