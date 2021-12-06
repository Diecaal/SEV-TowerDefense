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

		if (abs(actor->x - x) <= closestDistanceAllowedToActor
			&& abs(actor->y - y) <= closestDistanceAllowedToActor) {
			vx = 0;
			vy = 0;

			if (isMeleeAttacker)
				state = game->stateAttacking;
			else
				state = game->stateIdle;
		}
		else {
			vx = directionX * 1.0;
			vy = directionY * 1.0;
		}

	}
}

void BaseEnemy::setTimeLeftToMove(int timeLeft) {
	this->timeLeftToMove = timeLeft;
}

int BaseEnemy::impacted() {
	lifesLeft--;
	if (lifesLeft <= 0) {
		return points;
		if (state != game->stateDying) {
			state = game->stateDying;
		}
	}
	return 0;
}

void BaseEnemy::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}

void BaseEnemy::update() {
	if (timeLeftToMove > 0)
		timeLeftToMove--;

	if (timeLeftToMove <= 0 && state == game->stateIdle)
		state = game->stateMoving;

	if (lifesLeft <= 0)
		state = game->stateDying;

	if (state == game->stateDying) {
		animation = aDying;
	}

	if (state == game->stateAttacking) {
		animation = aAttacking;
	}

	// Establecer orientación
	if (vx > 0) {
		orientation = game->orientationRight;
	}
	if (vx < 0) {
		orientation = game->orientationLeft;
	}

	if (state == game->stateMoving) {
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aMovingRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aMovingLeft;
			}
		}
		if (vx == 0 && !isMeleeAttacker) {
			animation = aIdle;
		}
	}

	// Actualizar la animación
	bool endAnimation = animation->update();

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

bool BaseEnemy::canBeAttacked()
{
	return timeLeftToMove <= 0 && state != game->stateDying;
}
