#ifndef PACMAN_CLASS_H
#define PACMAN_CLASS_H

#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>

class Pacman {
public:

    void loadPacmanSprites();

    void showBody(int i, int posX, int posY, int flag);

    void animateBody(int posX, int posY, int dirX, int dirY, int mode);
};

#endif