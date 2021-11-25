#include "BaseCamp.h"

BaseCamp::BaseCamp(float x, float y, Game* game) 
	: ShootingActor("res/bloque_metal.png", x, y, game) {

}

void BaseCamp::looseLife() {
	this->lifes--;
}


