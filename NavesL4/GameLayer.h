#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "BaseEnemy.h"
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
	int points;
	int newEnemyTime = 0;
	void showCurrentWave();

	/* Tiles methods */
	void loadMap(string name);
	void loadMapObject(char character, float x, float y);
	int mapWidth, mapHeight;
	list<Cannon*> cannons;
	list<Tile*> tiles;
	struct variables /* Contains the variables that will be saved after reaching saving point */
	{
		public :
			const int NO_INFORMATION_ASSIGNED = -1;
		int baseCampLifesLeft = NO_INFORMATION_ASSIGNED;
		int points = NO_INFORMATION_ASSIGNED;
		list<Cannon*> activatedCannons;
	};
	struct variables savedVariables;

	/* Actors section */
	Player* player;
	Background* background;
	Background* backgroundNiebla;
	Actor* backgroundPoints;
	Actor* lifesLeftImage;
	list<SpawnerEnemy*> spawnerEnemies;
	list<BaseEnemy*> enemies;
	list<Projectile*> projectiles;
	int recolectableItems;
	Actor* message;
	bool pause;
	bool controlContinue = false;
	// Elementos de interfaz
	Actor* buttonShoot;
	Pad* pad;
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
	Text* currentWaveText;
};

