#pragma once

#include "ShootingAction.h"
#include "Projectile.h"
#include "Actor.h"
#include <cmath>
#include <list>

class Cannon : public Actor
{
public:
	Cannon(float x, float y, Game* game);
	ShootingAction* shootingAction;
	bool activated;
	void setActivated();
	int lifes = 3;
};

