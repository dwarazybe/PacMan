#ifndef GHOST_CLASS_H
#define GHOST_CLASS_H

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include "Map.h"

class Ghost : public Map {
public:
    enum directions { UP, DOWN, LEFT, RIGHT };
    ALLEGRO_BITMAP* ghost;
    int positionX, positionY, chunkPosX, chunkPosY, dir = 0, prevDir = 0;
    bool alive;
    bool firstMove = true;

    Ghost(int x, int y);

    void drawGhost();

    void findDir(int prevDir);

    void move();
};

#endif