#include "SpawnerEnemy.h"

SpawnerEnemy::SpawnerEnemy(float x, float y, Game* game)
	: Actor("res/enemigo2.png", x, y, 46, 34, game) {

	timeToGenerate = 100;

	vx = 1;
	vxIntelligence = -1;
	vx = vxIntelligence;
	
	state = game->stateMoving;
}

void SpawnerEnemy::update() {
	// Establecer velocidad
	if (state != game->stateDying) {
		// no está muerto y se ha quedado parado
		if (vx == 0) {
			vxIntelligence = vxIntelligence * -1;
			vx = vxIntelligence;
		}
		if (outRight) {
			// mover hacia la izquierda vx tiene que ser negativa
			if (vxIntelligence > 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
		if (outLeft) {
			// mover hacia la derecha vx tiene que ser positiva
			if (vxIntelligence < 0) {
				vxIntelligence = vxIntelligence * -1;
			}
			vx = vxIntelligence;
		}
	}
	else {
		vx = 0;
	}
}

void SpawnerEnemy::impacted() {
	if (state != game->stateDying) {
		state = game->stateDying;
	}
}

list<Enemy*> SpawnerEnemy::spawnEnemies() {
	list<Enemy*> ret;
	// Used for more realistic random values in X axis
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distrX(x-80, x+80); 
	std::uniform_int_distribution<int> distrVY(8, 18);
	for (int i = 0; i < enemiesToGenerate; i++) {
		int newEnemyX = distrX(eng);
		Enemy* newEnemy = new Enemy(newEnemyX, y - 10, game);
		newEnemy->vy = -1 * distrVY(eng);
		ret.push_back(newEnemy);
	}
	timeToGenerate = 300;
	return ret;
}