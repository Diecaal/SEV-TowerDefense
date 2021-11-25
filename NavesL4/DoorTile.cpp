#include "DoorTile.h"

DoorTile::DoorTile(int id, string filename, float x, float y, Game* game)
	: Tile(filename, x, y, game) {
	
	doorLinkedID = id;
	coolDownTime = 0;
}

void DoorTile::update() {
	if (coolDownTime > 0) {
		coolDownTime--;
	}
}

void DoorTile::draw(float scrollX, float scrollY) {
	if (coolDownTime == 0) {
		Tile::draw(scrollX, scrollY);
	}
	else {
		if (coolDownTime % 10 >= 0 && coolDownTime % 10 <= 5) {
			Tile::draw(scrollX, scrollY);
		}
	}
}