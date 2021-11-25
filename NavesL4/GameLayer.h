#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "SpawnerEnemy.h"
#include "Projectile.h"
#include "Text.h"
#include <list>
#include "Tile.h"
#include "DoorTile.h"
#include <fstream> // Leer ficheros
#include <sstream> // Leer líneas / String
#include "Space.h" 
#include "Pad.h"
#include "Recolectable.h";
#include "BaseCamp.h";
#include "Cannon.h";
#include <map>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;

	void keysToControls(SDL_Event event);
	void mouseToControls(SDL_Event event); // USO DE MOUSE
	Text* lifePoints;
	Text* textPoints;
	Text* recolectablesPoints;
	int points;
	int newEnemyTime = 0;

	/* Tiles methods */
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	int mapWidth, mapHeight;
	list<Cannon*> cannons;
	list<Tile*> tiles;
	Tile* cup; // Elemento de final de nivel
	Tile* savingPoint;
	const int NO_POSITION_ASSIGNED = -1;
	struct variables /* Contains the variables that will be saved after reaching saving point */
	{
		const int NO_POSITION_ASSIGNED = -1;
		int savingCoords[2] = { NO_POSITION_ASSIGNED, NO_POSITION_ASSIGNED }; // [0]->x / [1]->y
		int lifesLeft = NO_POSITION_ASSIGNED;
	};
	struct variables savedVariables;

	/* Actors section */
	Player* player;
	Background* background;
	Actor* backgroundPoints;
	Actor* lifesLeftImage;
	Actor* recolectablesImage;
	list<Recolectable*> recolectables;
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	int recolectableItems;
	Actor* message;
	bool pause;
	bool controlContinue = false;
	// Elementos de interfaz
	Actor* buttonJump;
	Actor* buttonShoot;
	Pad* pad;
	SpawnerEnemy* spawnerEnemy;
	BaseCamp* baseCamp;

	/* Scroll section*/
	void calculateScroll();
	float scrollX;
	float scrollY;

	bool controlShoot = false;
	int controlMoveY = 0;
	int controlMoveX = 0;

	Space* space;
	void checkCannonCreation();
	void assignEnemiesTimeLeftToMove();
};

