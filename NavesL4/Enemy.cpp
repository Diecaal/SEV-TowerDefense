#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: BaseEnemy("res/enemigo.png", x, y, 46, 46, game) {
	
	aDying = new Animation("res/knight_death_animation.png", width, height,
		378, 42, 6, 9, false, game);
	aMoving = new Animation("res/knight_walk_animation.png", width, height,
		336, 42, 6, 8, true, game);
	aIdle = new Animation("res/knight_idle.png", width, height,
		168, 42, 6, 4, true, game);

	isMeleeAttacker = true;
	
	animation = aIdle; // la animacion que se va a pintar
	state = game->stateIdle;
}

void Enemy::update() {
	
	BaseEnemy::update();
}
