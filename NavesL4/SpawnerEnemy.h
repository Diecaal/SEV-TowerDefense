#pragma once

#include "BaseEnemy.h"
#include "Animation.h"
#include "Enemy.h"
#include <list>
#include <random>

class SpawnerEnemy : public BaseEnemy
{
public:
	SpawnerEnemy(float x, float y, Game* game);
	list<Enemy*> spawnEnemies();
	void update();
	int enemiesToGenerate = 3;
	int timeToGenerate;
	int impactedEffectTime = 0;
	int impacted();
	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
};

