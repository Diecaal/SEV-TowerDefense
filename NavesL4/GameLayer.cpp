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
	if (game->currentLevel == 0) {
		audioBackground = new Audio("res/musica_ambiente.mp3", true);
		audioBackground->play();
	}

	pad = new Pad(WIDTH * 0.15, HEIGHT * 0.80, game);
	buttonShoot = new Actor("res/boton_disparo.png", WIDTH * 0.85, HEIGHT * 0.83, 100, 100, game);

	space = new Space(0);
	scrollX = 0;
	scrollY = 0;
	tiles.clear();
	
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	
	background = new Background("res/mapa_diejin_720x480.jpg", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundNiebla = new Background("res/niebla_hueca_720x480.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game); // WIDTH/HEIGHT * x -> mas sencillo cuadrar en pantalla

	currentWaveText = new Text("Wave here", WIDTH * 0.26, HEIGHT * 0.06, game);
	currentWaveText->content = "WAVE: " + to_string(game->currentLevel + 1) + "/" + to_string(game->finalLevel+1);

	spawnerEnemies.clear();
	enemies.clear(); // Vaciar por si reiniciamos el juego
	cannons.clear();
	lifeGenerators.clear();
	projectiles.clear(); // Vaciar por si reiniciamos el juego

	loadMap("res/" + to_string(game->currentLevel) + ".txt");

	lifesLeftImage = new Actor("res/corazon.png", WIDTH * 0.07, HEIGHT * 0.06, 44, 44, game);
	lifePoints = new Text("Lifes here", WIDTH * 0.13, HEIGHT * 0.06, game);
	
	// En caso de que el jugador ya ha jugado y vuelve a la wave 0, resetear los items guardados
	if (game->currentLevel == 0) {
		clearSavedVariables();
	}

	/* Carga entre niveles de los items de niveles anteriores */
	if (savedVariables.points != savedVariables.NO_INFORMATION_ASSIGNED) {
		baseCamp->lifes = savedVariables.baseCampLifesLeft;
		points = savedVariables.points;
		for (auto const& savedCannon : savedVariables.activatedCannons) {
			for (auto const& gameCannon : cannons) {
				if (savedCannon->x == gameCannon->x && savedCannon->y == gameCannon->y) {
					gameCannon->setActivated();
				}
			}
		}
		for (auto const& savedLifeGenerator : savedVariables.activatedLifeGenerators) {
			for (auto const& lifeGeneratorGame : lifeGenerators) {
				if (savedLifeGenerator->x == lifeGeneratorGame->x && savedLifeGenerator->y == lifeGeneratorGame->y) {
					if(baseCamp->lifes < baseCamp->maxLifes)
						baseCamp->lifes++;
					lifeGeneratorGame->setActivated();
				}
			}
		}
	}

	textPoints->content = to_string(points);
	lifePoints->content = to_string(baseCamp->lifes);
}

void GameLayer::update() {
	if (pause) {
		return;
	}

	space->update();

	player->update();
	for (auto const& enemy : enemies) {
		enemy->actorToFollow(baseCamp);
		enemy->update();
	}

	// Nivel superado - todos los enemigos de la ronda han sido eliminados
	if (enemies.size() <= 0) {
		savedVariables.points = points;
		for (auto const& cannon : cannons) {
			if (cannon->activated) {
				savedVariables.activatedCannons.push_back(cannon);
			}
		}
		for (auto const& lifeGenerator : lifeGenerators) {
			if (lifeGenerator->activated) {
				savedVariables.activatedLifeGenerators.push_back(lifeGenerator);
			}
		}
		// Por cada oleada se le dara una vida al jugador
		if (baseCamp->lifes < baseCamp->maxLifes) {
			savedVariables.baseCampLifesLeft = baseCamp->lifes+1;
		}
		else {
			savedVariables.baseCampLifesLeft = baseCamp->lifes;
		}
		game->currentLevel++;
		if (game->currentLevel > game->finalLevel) {
			game->currentLevel = 0;
		}
		message = new Actor("res/mensaje_ganar.png", WIDTH * 0.5, HEIGHT * 0.5,
			WIDTH-150, HEIGHT-150, game);
		pause = true;
		init();
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

	/* Colisiones - Juego perdido */
	for (auto const& enemy : enemies) {
		/* Enemigos melee atacan a la base */
		if (enemy->state == game->stateAttacking) {
			if (enemy->currentAttackingCoolDown <= 0) {
				baseCamp->loseLife();
				lifePoints->content = to_string(baseCamp->lifes);
				enemy->currentAttackingCoolDown = enemy->attackingCoolDown;
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
			else {
				enemy->currentAttackingCoolDown--;
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
				// Apuntara el caño a la posicion donde hara el disparo
				cannon->setLastProjectileCoords(newProjectile);
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

	/* Colisiones enemigos con proyectiles */
	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile) && enemy->canBeAttacked() && projectile->canDealDamage) {
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

void GameLayer::checkLifeGeneratorCreation() {
	/* Comprobar la creacion de un nuevo cannon */
	for (auto const& lifeGenerator : lifeGenerators) {
		if (player->isOverlap(lifeGenerator)) {
			if (points >= 5) {
				points -= 5;
				textPoints->content = to_string(points);
				lifeGenerator->setActivated();
			}
		}
	}
}

void GameLayer::draw() {
	background->draw();

	for (auto const& tile : tiles) {
		tile->draw(scrollX, scrollY);
	}

	for (auto const& projectile : projectiles) {
		projectile->draw(scrollX, scrollY);
	}

	for (auto const& lifeGenerator : lifeGenerators) {
		lifeGenerator->draw(scrollX, scrollY);
	}

	baseCamp->draw(scrollX, scrollY);

	for (auto const& enemy : enemies) {
		enemy->draw(scrollX, scrollY);
	}

	for (auto const& cannon : cannons) {
		cannon->draw(scrollX, scrollY);
	}	

	player->draw(scrollX, scrollY);

	// HUD
	if (game->input == game->inputMouse) {
		buttonShoot->draw(); // NO TIENEN SCROLL, POSICION FIJA
		pad->draw(); // NO TIENEN SCROLL, POSICION FIJA
	}

	backgroundNiebla->draw();
	textPoints->draw();
	lifePoints->draw();
	lifesLeftImage->draw();
	// Pintamos el HUD al final
	backgroundPoints->draw();
	currentWaveText->draw();

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
	int currentWave = 0;
	int numberOfWavesPerLevel = 2;
	int wavesTimeAwait[] = {50, 200};
	int enemiesPerWave = enemies.size() / numberOfWavesPerLevel;
	// Asignar automaticamente tamaño a 1 cuando haya pocos enemigos
	if (enemiesPerWave == 0) enemiesPerWave = 1;
	int currentEnemy = 0;
	for (auto const& enemy : enemies) {
		currentEnemy++;
		if ((currentEnemy - (enemiesPerWave * currentWave)) >= enemiesPerWave) {
			currentWave++;
		}
		enemy->timeLeftToMove = wavesTimeAwait[currentWave];
	}
}

void GameLayer::loadMapObject(char character, float x, float y)
{
	switch (character) {
		case '1': {
			player = new Player(x, y, game);
			space->mainPlayer = player;
			break;
		}
		case 'C': {
			Cannon* cannon = new Cannon(x, y, game);
			cannons.push_back(cannon);
			break;
		}
		case 'E': {
			Enemy* enemy = new Enemy(x, y, game);
			enemies.push_back(enemy);
			space->addDynamicActor(enemy);
			break;
		}
		case 'B': {
			baseCamp = new BaseCamp(x, y, game);
			baseCamp->y = baseCamp->y - baseCamp->height / 2;
			break;
		}
		case 'S': {
			SpawnerEnemy* spawnerEnemy = new SpawnerEnemy(x, y, game);
			enemies.push_back(spawnerEnemy);
			spawnerEnemies.push_back(spawnerEnemy);
			space->addDynamicActor(spawnerEnemy);
			break;
		}
		case 'M': {
			Tile* tile = new Tile("res/bloque_fondo_muro.png", x, y, game);
			tiles.push_back(tile);
			space->addStaticActor(tile);
			break;
		}
		case 'L': {
			LifeGenerator* lifeGenerator = new LifeGenerator(x, y, game);
			lifeGenerators.push_back(lifeGenerator);
			break;
		}
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
		controlContinue = true;
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
			checkLifeGeneratorCreation();
			break;
		case SDLK_SPACE: // dispara
			if(!pause)
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
			controlMoveY = pad->getOrientationY(motionY);
		}
		// Comprobamo si el click se ha hecho dentro de nuestro actor para disparar
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = true;
		}
	}
	// Cada vez que se mueve en cualquier lugar de la pantalla
	if (event.type == SDL_MOUSEMOTION) {
		if (buttonShoot->containsPoint(motionX, motionY) == false) {
			controlShoot = false;
		}
		if (pad->clicked && pad->containsPoint(motionX, motionY)) {
			controlMoveX = pad->getOrientationX(motionX);
			controlMoveY = pad->getOrientationY(motionY);
			// Rango de -20 a 20 es igual que 0
			if (controlMoveX > -20 && controlMoveX < 20) {
				controlMoveX = 0;
			}
			if (controlMoveY > -20 && controlMoveY < 20) {
				controlMoveY = 0;
			}
			else {
				pad->clicked = false; // han sacado el ratón del pad
				controlMoveX = 0;
				controlMoveY = 0;
			}

		}
	}
	// Cada vez que levantan el click
	if (event.type == SDL_MOUSEBUTTONUP) {
		if (pad->containsPoint(motionX, motionY)) {
			pad->clicked = false;
			// LEVANTAR EL CLICK TAMBIEN TE PARA
			controlMoveX = 0;
			controlMoveY = 0;
		}
		if (buttonShoot->containsPoint(motionX, motionY)) {
			controlShoot = false;
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

void GameLayer::clearSavedVariables() {
	savedVariables.activatedCannons.clear();
	savedVariables.activatedCannons.clear();
	savedVariables.baseCampLifesLeft = baseCamp->maxLifes;
	savedVariables.points = 0;
}