#pragma once

// Todas las librerias de SDL y otras utilidades
#include <iostream>
#include <string>
using namespace std;
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map> 

// Valores generales
#define WIDTH 720
#define HEIGHT 480

#include "Layer.h"
class Layer;

class Game
{
public:
	Game();
	void scale();
	bool scaledToMax = false;
	float scaleLower = 1;
	void loop();
	SDL_Texture* getTexture(string filename);
	map<string, SDL_Texture*> mapTextures; // map - cache
	SDL_Window* window; // ventana
	SDL_Renderer* renderer; // renderizador
	bool loopActive; // Juego activo
	TTF_Font* font;

	/* Different layers for the game */
	Layer* layer;
	Layer* menuLayer;
	Layer* gameLayer;

	/* Game levels */
	int currentLevel = 0;
	int finalLevel = 3;

	/* Input sectino */
	int input;
	int const inputKeyboard = 1;
	int const inputMouse = 2;
	int const inputGamePad = 3;

	/* States of the player */
	int const stateMoving = 1;
	int const stateJumping = 2;
	int const stateDying = 3;
	int const stateDead = 4;
	int const stateShooting = 5;

	/* Orientation for the actors */
	int const orientationRight = 1;
	int const orientationLeft = 2;
};
