#include "BaseCamp.h"

BaseCamp::BaseCamp(float x, float y, Game* game) 
	: Actor("res/bloque_metal.png", x, y, 40, 32, game) {

	this->shootingAction = new ShootingAction(this, game);
}

void BaseCamp::loseLife() {
	this->lifes--;
}


