#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo.png", x, y, 33, 19, game) {
	vx = 9;
}

Projectile::Projectile(float x, float y, Game* game, float newVx, float newVy) :
	Actor("res/disparo.png", x, y, 33, 19, game) {
	this->vx = newVx;
	this->vy = newVy;
}
