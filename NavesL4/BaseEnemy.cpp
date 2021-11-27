#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(string fileName, int x, int y, int width, int height, Game* game) :
	Actor(fileName, x, y, width, height, game)
{
	animation = aIdle; // la animacion que se va a pintar
	state = game->stateIdle;
}

void BaseEnemy::actorToFollow(Actor* actor) {
	if (timeLeftToMove <= 0) {
		float directionX = actor->x - x;
		float directionY = actor->y - y;
		/* Normalize vector */
		float hypotenus = sqrt(pow(directionX, 2) + pow(directionY, 2));
		directionX = directionX / hypotenus;
		directionY = directionY / hypotenus;

		if (isMeleeAttacker) {
			vx = directionX * 1.2;
			vy = directionY * 1.2;
		}
		else {
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
}

void BaseEnemy::setTimeLeftToMove(int timeLeft) {
	this->timeLeftToMove = timeLeft;
}

void BaseEnemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}

void BaseEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void BaseEnemy::update() {
	if (timeLeftToMove > 0)
		timeLeftToMove--;

	// Actualizar la animación
	bool endAnimation = animation->update();
	if (timeLeftToMove <= 0 && state == game->stateIdle)
		this->state = game->stateMoving;

	if (state == game->stateMoving) {
		animation = aMoving;
	}
	if (state == game->stateDying) {
		animation = aDying;
	}

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba muriendo
		if (state == game->stateDying) {
			state = game->stateDead;
		}
	}

	// Si el enemigo esta muriendo no se movera
	if (state == game->stateDying) {
		vx = 0;
		vy = 0;
	}
}