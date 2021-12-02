#pragma once

#include "ShootingAction.h"
#include "Projectile.h"
#include "Actor.h"
#include <cmath>
#include <list>
#include <math.h>

class Cannon : public Actor
{
public:
	Cannon(float x, float y, Game* game);
	ShootingAction* shootingAction;
	void setLastProjectileCoords(Projectile* projectile);
	float lastVx = -1;
	float lastVy = -1;
	void draw(float scrollX = 0, float scrollY = 0) override;
	bool activated;
	void setActivated();
	int lifes = 3;
};

