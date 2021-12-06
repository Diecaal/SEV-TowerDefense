#include "Enemy.h"

Enemy::Enemy(float x, float y, Game* game)
	: BaseEnemy("res/enemigo.png", x, y, 46, 46, game) {
	
	this->points = 1;

	aDying = new Animation("res/knight_death_animation.png", width, height,
		378, 42, 6, 9, false, game);
	aMovingRight = new Animation("res/knight_walk_animation_right.png", width, height,
		336, 42, 6, 8, true, game);
	aMovingLeft = new Animation("res/knight_walk_animation_left.png", width, height,
		336, 42, 6, 8, true, game);
	aIdle = new Animation("res/knight_idle.png", width, height,
		168, 42, 6, 4, true, game);
	aAttacking = new Animation("res/knight_attack.png", width, height,
		418, 42, 4, 10, true, game);

	isMeleeAttacker = true;

	animation = aIdle; // la animacion que se va a pintar
	state = game->stateIdle;

	closestDistanceAllowedToActor = 70;
	attackingCoolDown = 40;
	currentAttackingCoolDown = attackingCoolDown;
}

void Enemy::update() {
	
	BaseEnemy::update();
}
