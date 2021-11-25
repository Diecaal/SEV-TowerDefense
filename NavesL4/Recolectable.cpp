#include "Recolectable.h"

Recolectable::Recolectable(float x, float y, Game* game)
	: Actor("res/icono_recolectable.png", x, y, 40, 40, game) {

	animation = new Animation("res/recolectable.png", width, height, 
		256, 32, 6, 8, true, game);

	state = game->stateMoving;
}

void Recolectable::update() {
	animation->update();
}

void Recolectable::draw(float scrollX, float scrollY) {
	animation->draw(x - scrollX, y - scrollY);
}
