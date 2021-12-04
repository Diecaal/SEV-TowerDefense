#include "Cannon.h"

Cannon::Cannon(float x, float y, Game* game)
	: Actor("res/cannon_unbuilt.png", x, y, 38, 38, game) {

	this->shootingAction = new ShootingAction(this, game);
}

void Cannon::setActivated() {
	this->width = 38;
	this->height = 51;
	this->fileWidth = width;
	this->fileHeight = height;
	this->texture = game->getTexture("res/canon.png");
	this->activated = true;
}

void Cannon::setLastProjectileCoords(Projectile* projectile) {
	lastVx = projectile->vx;
	lastVy = projectile->vy;
}

void Cannon::draw(float scrollX, float scrollY) {
	// Recorte en el fichero de la imagen
	SDL_Rect source;
	source.x = 0;
	source.y = 0;
	source.w = fileWidth; // texture.width;
	source.h = fileHeight; // texture.height;

	// Donde se va a pegar en el renderizador
	SDL_Rect destination;
	destination.x = x - width / 2 - scrollX;
	destination.y = y - height / 2 - scrollY;
	destination.w = width;
	destination.h = height;
	// Modificar para que la referencia sea el punto 
	if (lastVx != -1) {
		float hypotenus = sqrt(pow(lastVx, 2) + pow(lastVy, 2));
		lastVx = (lastVx / hypotenus);
		lastVy = (lastVy / hypotenus);

		double degrees = ((atan2(lastVy, lastVx) / M_PI) * 180.0) + 80.0;
		SDL_RenderCopyEx(game->renderer,
			texture, &source, &destination, degrees, NULL, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopyEx(game->renderer,
			texture, &source, &destination, 0, NULL, SDL_FLIP_NONE);
	}
}