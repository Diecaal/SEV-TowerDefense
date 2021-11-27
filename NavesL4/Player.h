#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Animation.h" // incluir animacion 

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	/* Todas las animaciones posibles */
	Animation* aIdleRight;
	Animation* aIdleLeft;
	Animation* aRunningRight;
	Animation* aRunningLeft;
	Animation* aShootingRight;
	Animation* aShootingLeft;
	Animation* aJumpingRight;
	Animation* aJumpingLeft;
	Animation* animation; // Referencia a la animación mostrada

	void update();
	void moveX(float axis);
	void moveY(float axis);
	void impacted();
	int shootCadence = 30;
	int shootTime = 0;
	int orientation;
	int state;
	int invulnerableTime = 0;
};

