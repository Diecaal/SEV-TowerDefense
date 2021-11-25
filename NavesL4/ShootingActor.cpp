#include "ShootingActor.h"

ShootingActor::ShootingActor(string fileName, float x, float y, Game* game)
	: Actor(fileName, x, y, 40, 32, game) {

}

/* Dispara al enemigo mas cercano */
Projectile* ShootingActor::shoot(list<Enemy*> enemies)
{
	if (enemies.size() == 0) {
		return NULL;
	}
	float closestX = -1;
	float closestY = -1;
	float closestDistance = numeric_limits<float>::max();
	for (Enemy* enemy : enemies) {
		/* Si el enemigo esta muriendo no lo tenemos en cuenta */
		if (enemy->state == game->stateMoving) {
			/* Comprobar si el enemigo esta en rango de tiro(circulo) */
			float distance = pow((enemy->x - x), 2) + pow((enemy->y - y), 2);
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
		shootingVector.x = closestX - x;
		shootingVector.y = closestY - y;
		/* Normalize vector */
		float hypotenus = sqrt(pow(shootingVector.x, 2) + pow(shootingVector.y, 2));
		shootingVector.x = (shootingVector.x / hypotenus) * 10;
		shootingVector.y = (shootingVector.y / hypotenus) * 10;
		shootingCooldown = 80;
		return new Projectile(x, y, game, shootingVector.x, shootingVector.y);
	}
}

bool ShootingActor::enemyInRange(list<Enemy*> enemies) {
	for (Enemy* enemy : enemies) {
		if (enemy->state != game->stateDying) {
			/* Comprobar si el enemigo esta en rango de tiro(circulo) */
			float distance = pow((enemy->x - x), 2) + pow((enemy->y - y), 2);
			bool isInShootingRange = (distance) <= pow(shootingRange, 2);
			if (isInShootingRange) {
				return true;
			}
		}
	}
	return false;
}

bool ShootingActor::shootAvailable() {
	return shootingCooldown <= 0;
}