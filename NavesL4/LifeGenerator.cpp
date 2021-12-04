#include "LifeGenerator.h"

LifeGenerator::LifeGenerator(float x, float y, Game* game)
	: Actor("res/lifeGenerator_unbuilt.png", x, y, 44, 44, game) {
		
}

void LifeGenerator::setActivated()
{
	this->width = 52;
	this->height = 47;
	this->fileWidth = width;
	this->fileHeight = height;
	this->texture = game->getTexture("res/lifeGenerator.png");
	this->activated = true;
}
