#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 48, 48, game) {

	audioShoot = new Audio("res/magic_spell.wav", false);

	orientation = game->orientationRight;
	state = game->stateMoving; // estado inicial sera moviendose

	aShootingRight = new Animation("res/player_attack_right.png", width, height, 
		423, 48, 4, 8, false, game);
	aShootingLeft = new Animation("res/player_attack_left.png",width, height, 
		420, 48, 4, 8, false, game);
	aIdleRight = new Animation("res/player_idle_right.png", width, height,
		273, 48, 6, 6, true, game);
	aIdleLeft = new Animation("res/player_idle_left.png", width, height,
		273, 48, 6, 6, true, game);
	aRunningRight = new Animation("res/player_run_right.png", width, height,
		403, 48, 6, 8, true, game);
	aRunningLeft = new Animation("res/player_run_left.png", width, height,
		402, 48, 6, 8, true, game);

	animation = aIdleRight;
}

void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	if (shootTime > 0) {
		shootTime--;
	}

	bool endAnimation = animation->update();
	
	// Selección de animación basada en estados
	if (state == game->stateAttacking) {
		if (orientation == game->orientationRight) {
			animation = aShootingRight;
		}
		if (orientation == game->orientationLeft) {
			animation = aShootingLeft;
		}
	}
	if (state == game->stateMoving) {
		// Establecer orientación
		if (vx > 0) {
			orientation = game->orientationRight;
		}
		if (vx < 0) {
			orientation = game->orientationLeft;
		}
		if (vx != 0) {
			if (orientation == game->orientationRight) {
				animation = aRunningRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aRunningLeft;
			}
		}
		if (vx == 0) {
			if (orientation == game->orientationRight) {
				animation = aIdleRight;
			}
			if (orientation == game->orientationLeft) {
				animation = aIdleLeft;
			}
		}
	}

	// Acabo la animación, no sabemos cual
	if (endAnimation) {
		// Estaba disparando
		if (state == game->stateAttacking) {
			state = game->stateMoving;
		}
	}
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime <= 0 && invulnerableTime <= 0) {
		state = game->stateAttacking;
		shootTime = shootCadence;
		aShootingLeft->currentFrame = 0; //"Rebobinar" animación
		aShootingRight->currentFrame = 0; //"Rebobinar" animación
		audioShoot->play();
		Projectile* projectile = new Projectile(x, y, game, true);
		if (orientation == game->orientationLeft) {
			projectile->vx = projectile->vx * -1; // Invertir
		}
		return projectile;
	}
	else {
		return NULL;
	}
}

void Player::draw(float scrollX, float scrollY) {
	if (invulnerableTime <= 0) {
		animation->draw(x - scrollX, y - scrollY);
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			animation->draw(x - scrollX, y - scrollY);
		}
	}
}

void Player::impacted() {
	if (invulnerableTime <= 0) {
		invulnerableTime = 100;
	}
}