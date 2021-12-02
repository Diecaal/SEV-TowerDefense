#include "BaseCamp.h"

BaseCamp::BaseCamp(float x, float y, Game* game) 
	: Actor("res/basecamp.png", x, y, 121, 132, game) {

	this->shootingAction = new ShootingAction(this, game);
}

void BaseCamp::loseLife() {
	this->lifes--;
}


