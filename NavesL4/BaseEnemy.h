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
	int currentAttackingCoolDown;
	int attackingCoolDown;

	/* Los enemigos se moveran cuando tengan a 0 timeLeftToMove */
	int timeLeftToMove;
	int lifesLeft = 1;
	int points;
	virtual int impacted(); // Recibe impacto y pone animación de morir
	bool isMeleeAttacker = false;
	int orientation;
	int closestDistanceAllowedToActor;

	/* Estados de enemigos */
	int state;
	Animation* aDying;
	Animation* aMovingRight;
	Animation* aMovingLeft;
	Animation* aIdle;
	Animation* aAttacking;
	Animation* animation; // Referencia a la animación mostrada

	void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
	virtual void update();
	bool canBeAttacked();
};

