#pragma once

#include "Actor.h"
#include "Animation.h"

class BaseEnemy : public Actor
{
public:
	BaseEnemy(string fileName, int x, int y, int width, int height, Game* game);
	void actorToFollow(Actor* actorToFollow);
	void setTimeLeftToMove(int timeLeft);
	float vxIntelligence;

	/* Los enemigos se moveran cuando tengan a 0 timeLeftToMove */
	int timeLeftToMove;
	int lifesLeft = 1;
	void impacted(); // Recibe impacto y pone animación de morir
	bool isMeleeAttacker = false;

	/* Estados de enemigos */
	int state;
	Animation* aDying;
	Animation* aMoving; //aShooting, aMoving...
	Animation* aIdle;
	Animation* animation; // Referencia a la animación mostrada

	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	virtual void update();
};

