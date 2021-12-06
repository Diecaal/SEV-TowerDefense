#include "ShootingAction.h"

ShootingAction::ShootingAction(Actor* currentActor, Game* game) {
	this->game = game;
	this->currentActor = currentActor;

	audioShoot = new Audio("res/cannon_sound.wav", false);
}

/* Dispara al enemigo mas cercano */
Projectile* ShootingAction::shoot(list<BaseEnemy*> enemies)
{
	if (enemies.size() == 0) {
		return NULL;
	}
	float closestX = -1;
	float closestY = -1;
	float closestDistance = numeric_limits<float>::max();
	for (BaseEnemy* enemy : enemies) {
		/* Si el enemigo esta muriendo no lo tenemos en cuenta */
		if (enemy->state == game->stateMoving) {
			/* Comprobar si el enemigo esta en rango de tiro(circulo) */
			float distance = pow((enemy->x - currentActor->x), 2) + pow((enemy->y - currentActor->y), 2);
			bool isInShootingRange = (distance) <= pow(shootingRange, 2);

			if (isInShootingRange) {
				if (distance < closestDistance) {
					closestDistance = distance;
					if (enemy->x > WIDTH / 2)
						closestX = enemy->x - enemy->width / 2;
					else
						closestX = enemy->x + enemy->width / 2;
					closestY = enemy->y;
				}
			}
		}
	}

	if (closestX != -1) {
		shootingVector.x = closestX - currentActor->x;
		shootingVector.y = closestY - currentActor->y;
		/* Normalize vector */
		float hypotenus = sqrt(pow(shootingVector.x, 2) + pow(shootingVector.y, 2));
		shootingVector.x = (shootingVector.x / hypotenus) * 10;
		shootingVector.y = (shootingVector.y / hypotenus) * 10;
		shootingCooldown = 80;
		audioShoot->play();
		return new Projectile(currentActor->x, currentActor->y, game, shootingVector.x, shootingVector.y);
	}
}

bool ShootingAction::enemyInRange(list<BaseEnemy*> enemies) {
	for (BaseEnemy* enemy : enemies) {
		if (enemy->state != game->stateDying) {
			/* Comprobar si el enemigo esta en rango de tiro(circulo) */
			float distance = pow((enemy->x - currentActor->x), 2) + pow((enemy->y - currentActor->y), 2);
			bool isInShootingRange = (distance) <= pow(shootingRange, 2);
			if (isInShootingRange) {
				return true;
			}
		}
	}
	return false;
}

bool ShootingAction::shootAvailable() {
	return shootingCooldown <= 0;
}