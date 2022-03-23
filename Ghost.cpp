#include "Ghost.h"

enum directions { UP, DOWN, LEFT, RIGHT };
ALLEGRO_BITMAP* ghost;
int positionX, positionY, chunkPosX, chunkPosY, dir = 0, prevDir = 0;
bool alive;
bool firstMove = true;

Ghost::Ghost(int x, int y) {
    positionX = x;
    positionY = y;
    chunkPosX = 0;
    chunkPosY = 0;
    alive = true;
    ghost = al_load_bitmap("images/ghost.png");
}

void Ghost::drawGhost() {
    al_draw_bitmap(ghost, positionX * CHUNK_SIZE + 8 * chunkPosX, positionY * CHUNK_SIZE + 8 * chunkPosY, 0);
}

void Ghost::findDir(int prevDir) {
    int dirX = 0, dirY = 0;

    if (dir == prevDir) {
        if (prevDir == UP || prevDir == DOWN) {
            int newDir = rand() % 2;
            dir = newDir + 2;
        }
        if (prevDir == LEFT || prevDir == RIGHT) {
            int newDir = rand() % 2;
            dir = newDir;
        }
    }

    if (firstMove) {
        dir = UP;
        firstMove = false;
    }
    else {
        dir = rand() % 4;
    }
    if (dir == UP) {
        dirX = 0;
        dirY = -1;
    }

    if (dir == DOWN) {
        dirX = 0;
        dirY = 1;
    }

    if (dir == LEFT) {
        dirX = -1;
        dirY = 0;
    }

    if (dir == RIGHT) {
        dirX = 1;
        dirY = 0;
    }
    if (chunkPosX == 0 && chunkPosY == 0) {
        if (mapChunks[positionY + dirY][positionX + dirX] == 0) {
            return;
        }
        else {
            return findDir(prevDir);
        }
    }
}

void Ghost::move() {
    int dirX = 0, dirY = 0;
    int change;
    int prevDir = UP;
        if (dir == UP) {
            prevDir = DOWN;
            dirX = 0;
            dirY = -1;
        }
        if (dir == DOWN) {
            prevDir = UP;
            dirX = 0;
            dirY = 1;
        }
        if (dir == LEFT) {
            prevDir = RIGHT;
            dirX = -1;
            dirY = 0;
        }
        if (dir == RIGHT) {
            prevDir = LEFT;
            dirX = 1;
            dirY = 0;
        }

        if (chunkPosX == 0 && chunkPosY == 0) {
            if (mapChunks[positionY + dirY][positionX + dirX] == 0) {
                change = rand() % 2;
                if (change == 1) {
                    findDir(prevDir);
                }
            }
            else {
                findDir(prevDir);
            }

            if (dir == UP) {
                dirX = 0;
                dirY = -1;
            }
            if (dir == DOWN) {
                dirX = 0;
                dirY = 1;
            }
            if (dir == LEFT) {
                dirX = -1;
                dirY = 0;
            }
            if (dir == RIGHT) {
                dirX = 1;
                dirY = 0;
            }
        }
        chunkPosX += dirX;
        chunkPosY += dirY;

        if (chunkPosX > 1 || chunkPosX < -2 || chunkPosY > 1 || chunkPosY < -2) {

            if (chunkPosX > 1) {
                chunkPosX = -2;
            }
            else if (chunkPosX < -2) {
                chunkPosX = 1;
            }
            else if (chunkPosY > 1) {
                chunkPosY = -2;
            }
            else {
                chunkPosY = 1;
            }

            positionX += dirX;
            positionY += dirY;

            if (positionX == 18 || positionX == 0) {

                if (dirX > 0)
                    positionX = 0;
                else
                    positionX = 18;
            }

            if (positionY == 20 || positionY == 0) {
                if (dirY > 0)
                    positionY = 0;
                else
                    positionY = 20;
            }
        }
    }