#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	pause = true;
	message = new Actor("res/mensaje_como_jugar.png", WIDTH * 0.5, HEIGHT * 0.5,
		WIDTH, HEIGHT, game);
	init();
}

void GameLayer::init() {
	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	buttonJump = new Actor("res/boton_salto.png", WIDTH * 0.9, HEIGHT * 0.55, 100, 100, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.75, HEIGHT * 0.83, 100, 100, game);
	points = 0;

	space = new Space(0);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();

	// Resetear el struct con variables guardadas
	savedVariables.savingCoords[0] = NO_POSITION_ASSIGNED;
	savedVariables.savingCoords[1] = NO_POSITION_ASSIGNED;
	savedVariables.lifesLeft = 3;

	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points); // C++ no cambia datos automaticamente bien

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game); // WIDTH/HEIGHT * x -> mas sencillo cuadrar en pantalla

	currentWave = 0;
	currentWaveText = new Text("Wave here", WIDTH * 0.35, HEIGHT * 0.07, game);
	currentWaveText->content = to_string(currentWave);

	spawnerEnemies.clear();
	enemies.clear(); // Vaciar por si reiniciamos el juego
	projectiles.clear(); // Vaciar por si reiniciamos el juego

	loadMap("res/" + to_string(game->currentLevel) + ".txt");

	lifesLeftImage = new Actor("res/icono_vidas.png", WIDTH * 0.07, HEIGHT * 0.08, 40, 40, game);
	lifePoints = new Text("Lifes here", WIDTH * 0.14, HEIGHT * 0.07, game);
	lifePoints->content = to_string(baseCamp->lifes);
}

void GameLayer::update() {
	if (pause) {
		return;
	}

	space->update();
	//background->update();

	player->update();
	for (auto const& enemy : enemies) {
		enemy->actorToFollow(baseCamp);
		enemy->update();
	}

	/* Generacion de enemigos por los spawnerEnemies */
	for (auto const& spawnerEnemy : spawnerEnemies) {
		if (spawnerEnemy->state == game->stateMoving && spawnerEnemy->timeToGenerate <= 0) {
			for (auto const& enemy : spawnerEnemy->spawnEnemies()) {
				enemies.push_back(enemy);
				space->addDynamicActor(enemy);
			}
		}
	}

	/* Colisiones */
	for (auto const& enemy : enemies) {
		// If enemies are in the process of diying they wont kill you
		if (baseCamp->isOverlap(enemy) && enemy->state == game->stateMoving) {
			baseCamp->loseLife();
			lifePoints->content = to_string(baseCamp->lifes);
			if (baseCamp->lifes <= 0) {
				lifePoints->content = to_string(baseCamp->lifes);
				message = new Actor("res/mensaje_perder.png", WIDTH * 0.5, HEIGHT * 0.5,
					WIDTH, HEIGHT, game);
				pause = true;
				game->currentLevel = 0;
				init();
				break;
			}
		}
		if ((player->isOverlap(enemy) && enemy->state == game->stateMoving)) {
			player->impacted();
		}
	}

	/* Disparos de la base */
	if (baseCamp->shootingAction->shootAvailable() && baseCamp->shootingAction->enemyInRange(enemies)) {
		Projectile* newProjectile = baseCamp->shootingAction->shoot(enemies);
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}
	}
	else if (!baseCamp->shootingAction->shootAvailable()) {
		baseCamp->shootingAction->shootingCooldown--;
	}

	/* Disparo de los cannons */
	for (auto const& cannon : cannons) {
		if (cannon->activated && cannon->shootingAction->shootAvailable()) {
			Projectile* newProjectile = cannon->shootingAction->shoot(enemies);
			if (newProjectile != NULL) {
				space->addDynamicActor(newProjectile);
				projectiles.push_back(newProjectile);
			}
		}
		else if (!cannon->shootingAction->shootAvailable()) {
			cannon->shootingAction->shootingCooldown--;
		}
	}

	/* Colisiones , Enemy - Projectile */
	list<BaseEnemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;

	for (auto const& projectile : projectiles) {
		if (!projectile->isInRender(scrollX, scrollY)) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->isOverlap(baseCamp) && enemy->state == game->stateMoving) {
			enemy->impacted();
		}
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile) && enemy->state == game->stateMoving && projectile->canDealDamage) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					projectile->canDealDamage = false;
					deleteProjectiles.push_back(projectile);
				}

				enemy->impacted();
				points++;
				textPoints->content = to_string(points);
			}
		}
	}

	for (auto const& enemy : enemies) {
		if (enemy->state == game->stateDead) {
			bool eInList = std::find(deleteEnemies.begin(),
				deleteEnemies.end(),
				enemy) != deleteEnemies.end();

			if (!eInList) {
				deleteEnemies.push_back(enemy);
			}
		}
	}

	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
		space->removeDynamicActor(delEnemy);
	}
	deleteEnemies.clear();

	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		space->removeDynamicActor(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();
}

void GameLayer::checkCannonCreation() {
	/* Comprobar la creacion de un nuevo cannon */
	for (auto const& cannon : cannons) {
		if (player->isOverlap(cannon)) {
			if (points >= 5) {
				points -= 5;
				textPoints->content = to_string(points);
				cannon->setActivated();
			}
		}
	}
}

void GameLayer::draw() {
	//calculateScroll();

	background->draw();
	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	player->draw(scrollX, scrollY);
	baseCamp->draw(scrollX, scrollY);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	for (auto const& cannon : cannons) {
		cannon->draw(scrollX, scrollY);
	}

	textPoints->draw();
	lifePoints->draw();
	lifesLeftImage->draw();
	// Pintamos el HUD al final
	backgroundPoints->draw();
	currentWaveText->draw();

	// HUD
	if (game->input == game->inputMouse) {
		buttonJump->draw(); // NO TIENEN SCROLL, POSICION FIJA
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}
	if (pause) {
		message->draw();
	}

	SDL_RenderPresent(game->renderer); // Renderiza
}

void GameLayer::loadMap(string name) {
	char character;
	string line;
	ifstream streamFile(name.c_str());
	if (!streamFile.is_open()) {
		cout << "Falla abrir el fichero de mapa" << endl;
		return;
	}
	else {
		// Por línea
		for (int i = 0; getline(streamFile, line); i++) { // Filas
			istringstream streamLine(line);
			mapWidth = line.length() * 40; // Ancho del mapa en 
			// Por carácter (en cada línea)
			for (int j = 0; !streamLine.eof(); j++) { //Columnas
				streamLine >> character; // Leer character 
				cout << character;
				float x = 40 / 2 + j * 40; // x central
				float y = 32 + i * 32; // y suelo
				loadMapObject(character, x, y);
				if (streamLine.eof()) {
					mapHeight = 32 * j;
				}
			}
		}
		space->mapHeight = mapHeight;
		space->mapWidth = mapWidth;
	}
	streamFile.close();
	assignEnemiesTimeLeftToMove();
}

void GameLayer::assignEnemiesTimeLeftToMove() {
	int numberOfWaves = 3;
	int wavesTimeAwait[] = {100, 400, 700};
	int enemiesPerWave = enemies.size() / numberOfWaves;
	// Asignar automaticamente tamaño a 1 cuando haya pocos enemigos
	if (enemiesPerWave == 0) 
		enemiesPerWave = 1;
	int currentEnemy = 0;
	for (auto const& enemy : enemies) {
		currentEnemy++;
		if ((currentEnemy - (enemiesPerWave * currentWave)) >= enemiesPerWave) {
			currentWave++;
			currentWaveText->content = to_string(currentWave);
		}
		enemy->timeLeftToMove = wavesTimeAwait[currentWave];
	}
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
		case '1': {
			player = (savedVariables.savingCoords[0] == NO_POSITION_ASSIGNED) ?
				new Player(x, y, game) : 
				new Player(savedVariables.savingCoords[0], savedVariables.savingCoords[1], game);
			if (savedVariables.savingCoords[0] != NO_POSITION_ASSIGNED) {
				baseCamp->lifes = savedVariables.lifesLeft;
			}
			player->y = player->y - player->height / 2;
			space->mainPlayer = player;
			//space->addDynamicActor(player);
			break;
		}
		case 'C': {
			Cannon* cannon = new Cannon(x, y, game);
			cannon->y = cannon->y - cannon->height / 2;
			cannons.push_back(cannon);
			break;
		}
		case 'E': {
			Enemy* enemy = new Enemy(x, y, game);
			enemy->y = enemy->y - enemy->height / 2;
			enemies.push_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
		case 'B': {
			baseCamp = new BaseCamp(x, y, game);
			baseCamp->y = baseCamp->y - baseCamp->height / 2;
			//space->addStaticActor(baseCamp);
			break;
		}
		case 'S': {
			SpawnerEnemy* spawnerEnemy = new SpawnerEnemy(x, y, game);
			spawnerEnemy->y = spawnerEnemy->y - spawnerEnemy->height / 2;
			enemies.push_back(spawnerEnemy);
			spawnerEnemies.push_back(spawnerEnemy);
			space->addDynamicActor(spawnerEnemy);
			break;
		}
		case 'M': {
			Tile* tile = new Tile("res/bloque_fondo_muro.png", x, y, game);
			tile->y = tile->y - tile->height / 2;
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		/*
		case 'R': {
			Recolectable* recolectable = new Recolectable(x, y, game);
			// modificación para empezar a contar desde el suelo.
			recolectable->y = recolectable->y - recolectable->height / 2;
			recolectables.push_back(recolectable);
			break;
		}
		*/
	}
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		// Cambio automático de input
		if (event.type == SDL_KEYDOWN) {
			game->input = game->inputKeyboard;
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			game->input = game->inputMouse;
		}
		// Procesar teclas
		if (game->input == game->inputKeyboard) {
			keysToControls(event);
		}
		if (game->input == game->inputMouse) {
			mouseToControls(event);
		}
	}
	if (controlContinue) {
		pause = false;
		controlContinue = false;
	}

	// Disparar
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			space->addDynamicActor(newProjectile);
			projectiles.push_back(newProjectile);
		}

	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
	}

}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_c: // intentar creacion de cannon
			checkCannonCreation();
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}

	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}

	}

}

void GameLayer::mouseToControls(SDL_Event event) {
	// Modificación de coordenadas por posible escalado
	float motionX = event.motion.x / game->scaleLower;
	float motionY = event.motion.y / game->scaleLower;
	// Cada vez que hacen click
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		controlContinue = true;
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = true;
			// CLICK TAMBIEN TE MUEVE
			controlMoveX = pad->getOrientationX(motionX);
		}
		// Comprobamo si el click se ha hecho dentro de nuestro actor para disparar
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = -1;
		}
	}
	// Cada vez que se mueve en cualquier lugar de la pantalla
	if (event.type == SDL_MOUSEMOTION) {
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY) == false) {
			controlMoveY = 0;
		}
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}
			else {
				pad->clicked = false; // han sacado el ratón del pad
				controlMoveX = 0;
			}

		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
		}
		if (buttonJump->containsPoint(motionX, motionY)) {
			controlMoveY = 0;
		}
	}
}

void GameLayer::calculateScroll() {
	//scrollX = player->x - 200; //mas margen a la izquierda para ver al jugador
	// limite izquierda
	if (player->x > WIDTH * 0.3) {
		if (player->x - scrollX < WIDTH * 0.3) {
			scrollX = player->x - WIDTH * 0.3;
		}
	}

	// limite derecha
	if (player->x < mapWidth - WIDTH * 0.3) {
		if (player->x - scrollX > WIDTH * 0.7) {
			scrollX = player->x - WIDTH * 0.7;
		}
	}

	// SCROLL Y
	// limite superior
	if (player->y > HEIGHT * 0.3) {
		if (player->y - scrollY < HEIGHT * 0.3) {
			scrollY = player->y - HEIGHT * 0.3;
		}
	}

	// limite inferior
	if (player->y < mapHeight - HEIGHT * 0.3) {
		if (player->y - scrollY > HEIGHT * 0.5) {
			scrollY = player->y - HEIGHT * 0.6;
		}
	}

}
