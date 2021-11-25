#include "Cannon.h"

Cannon::Cannon(float x, float y, Game* game)
	: ShootingActor("res/bloque_tierra.png", x, y, game) {

}

void Cannon::setActivated() {
	this->texture = game->getTexture("res/bloque_fondo_muro.png");
	this->activated = true;
}