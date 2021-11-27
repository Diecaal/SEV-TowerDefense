#include "Cannon.h"

Cannon::Cannon(float x, float y, Game* game)
	: Actor("res/bloque_tierra.png", x, y, 40, 32, game) {

	this->shootingAction = new ShootingAction(this, game);
}

void Cannon::setActivated() {
	this->texture = game->getTexture("res/bloque_fondo_muro.png");
	this->activated = true;
}