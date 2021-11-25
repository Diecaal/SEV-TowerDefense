#pragma once

#include "Game.h"

class Actor
{
public:
	Actor(string filename, float x, float y, int width, int height, Game* game);
	virtual void draw(float scrollX = 0, float scrollY = 0);
	bool isOverlap(Actor* actor);
	bool isInRender(float scrollX = 0, float scrollY = 0);
	bool collisionDown;
	SDL_Texture* texture;
	float x;
	float y;
	float vx;
	float vy;
	float width;
	float height;
	int fileWidth;
	int fileHeight;
	Game* game; // referencia al juego
	bool outRight;
	bool outLeft;

	/* Uso del raton */
	bool containsPoint(int pointX, int pointY); // contiene punto
	bool clicked; // Está pulsado
};

