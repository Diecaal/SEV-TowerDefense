#pragma once
#include "Actor.h"
#include "Game.h"
#include "Animation.h"

class Recolectable : public Actor
{
public:
	Recolectable(float x, float y, Game* game);
	Animation* animation;
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	void update();
	int state;
};

