#pragma once
#include "Tile.h"
class DoorTile :
    public Tile
{
public:
    DoorTile(int doorLinkedID, string filename, float x, float y, Game* game);
    void update();
    void draw(float scrollX = 0, float scrollY = 0) override; // Va a sobrescribir
    int doorLinkedID;
    int coolDownTime;
};

