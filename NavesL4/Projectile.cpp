#include "Projectile.h"

Projectile::Projectile(float x, float y, Game* game) :
	Actor("res/disparo_jugador2.png", x, y, 20, 20, game) {
	vx = 9;
}

Projectile::Projectile(float x, float y, Game* game, float newVx, float newVy) :
	Actor("res/disparo_enemigo.png", x, y, 20, 20, game) {
	this->vx = newVx;
	this->vy = newVy;
}
