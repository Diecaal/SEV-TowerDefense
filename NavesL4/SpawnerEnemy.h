#pragma once

#include "Actor.h"
#include "Animation.h"
#include "Enemy.h"
#include <list>
#include <random>

class SpawnerEnemy : public Actor
{
public:
	SpawnerEnemy(float x, float y, Game* game);
	list<Enemy*> spawnEnemies();
	void update();
	void impacted();
	int enemiesToGenerate = 3;
	int timeToGenerate;
	float vxIntelligence;
	int state;
};

