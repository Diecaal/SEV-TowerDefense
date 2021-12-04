#pragma once

#include "Actor.h"

class LifeGenerator : public Actor
{
public:
	LifeGenerator(float x, float y, Game* game);
	bool activated;
	void setActivated();
};

