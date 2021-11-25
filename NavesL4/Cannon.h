#pragma once

#include "ShootingActor.h"
#include "Projectile.h"
#include <cmath>
#include <list>

class Cannon : public ShootingActor
{
public:
	Cannon(float x, float y, Game* game);
	bool activated;
	void setActivated();
	int lifes = 3;
};

