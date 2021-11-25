#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: Actor("res/enemigo.png", x, y, 36, 40, game) {

	state = game->stateMoving;

	aDying = new Animation("res/enemigo_morir.png", width, height,
		280, 40, 6, 8, false, game);
	aMoving = new Animation("res/enemigo_movimiento.png", width, height,
		108, 40, 6, 3, true, game);
	animation = aMoving; // la animacion que se va a pintar
}

void Enemy::actorToFollow(Actor* actor) {
	if (timeLeftToMove <= 0) {
		float directionX = actor->x - x;
		float directionY = actor->y - y;
		/* Normalize vector */
		float hypotenus = sqrt(pow(directionX, 2) + pow(directionY, 2));
		directionX = directionX / hypotenus;
		directionY = directionY / hypotenus;

		if (abs(actor->x - x) <= 60 && abs(actor->y - y) <= 60) {
			vx = 0;
			vy = 0;
		}
		else {
			vx = directionX * 1.2;
			vy = directionY * 1.2;
		}
	}
}

void Enemy::update() {
	// Actualizar la animación
	bool endAnimation = animation->update();
	
	// Si el enemigo esta muriendo no se movera
	if (state == game->stateDying) {
		vx = 0;
		vy = 0;
	}

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}


	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}
}

void Enemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}

void Enemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void Enemy::setTimeLeftToMove(int timeLeft) {
	this->timeLeftToMove = timeLeft;
}
