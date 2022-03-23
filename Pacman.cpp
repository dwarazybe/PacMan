#include "Pacman.h"

enum player_modes { DEFAULT, POWER, SAFE };
ALLEGRO_BITMAP* pacman[36];
int prevPosX = 0;
int prevPosY = 0;
int i = 0;
int CURRENT_FLAG = 0;

void Pacman::loadPacmanSprites() {
    if ((pacman[0] = al_load_bitmap("images/pacman_1.png")) == NULL
            || (pacman[1] = al_load_bitmap("images/pacman_2.png")) == NULL
            || (pacman[2] = al_load_bitmap("images/pacman_3.png")) == NULL
            || (pacman[3] = al_load_bitmap("images/pacman_4.png")) == NULL
            || (pacman[4] = al_load_bitmap("images/pacman_3.png")) == NULL
            || (pacman[5] = al_load_bitmap("images/pacman_2.png")) == NULL
            || (pacman[6] = al_load_bitmap("images/pacman_1.png")) == NULL
            || (pacman[7] = al_load_bitmap("images/pacman_5.png")) == NULL
            || (pacman[8] = al_load_bitmap("images/pacman_6.png")) == NULL
            || (pacman[9] = al_load_bitmap("images/pacman_7.png")) == NULL
            || (pacman[10] = al_load_bitmap("images/pacman_6.png")) == NULL
            || (pacman[11] = al_load_bitmap("images/pacman_5.png")) == NULL
            || (pacman[12] = al_load_bitmap("images/pacman_powered_1.png")) == NULL
            || (pacman[13] = al_load_bitmap("images/pacman_powered_2.png")) == NULL
            || (pacman[14] = al_load_bitmap("images/pacman_powered_3.png")) == NULL
            || (pacman[15] = al_load_bitmap("images/pacman_powered_4.png")) == NULL
            || (pacman[16] = al_load_bitmap("images/pacman_powered_3.png")) == NULL
            || (pacman[17] = al_load_bitmap("images/pacman_powered_2.png")) == NULL
            || (pacman[18] = al_load_bitmap("images/pacman_powered_1.png")) == NULL
            || (pacman[19] = al_load_bitmap("images/pacman_powered_5.png")) == NULL
            || (pacman[20] = al_load_bitmap("images/pacman_powered_6.png")) == NULL
            || (pacman[21] = al_load_bitmap("images/pacman_powered_7.png")) == NULL
            || (pacman[22] = al_load_bitmap("images/pacman_powered_6.png")) == NULL
            || (pacman[23] = al_load_bitmap("images/pacman_powered_5.png")) == NULL
            || (pacman[24] = al_load_bitmap("images/pacman_powered_8.png")) == NULL
            || (pacman[25] = al_load_bitmap("images/pacman_powered_9.png")) == NULL
            || (pacman[26] = al_load_bitmap("images/pacman_powered_10.png")) == NULL
            || (pacman[27] = al_load_bitmap("images/pacman_powered_11.png")) == NULL
            || (pacman[28] = al_load_bitmap("images/pacman_powered_10.png")) == NULL
            || (pacman[29] = al_load_bitmap("images/pacman_powered_9.png")) == NULL
            || (pacman[30] = al_load_bitmap("images/pacman_powered_8.png")) == NULL
            || (pacman[31] = al_load_bitmap("images/pacman_powered_12.png")) == NULL
            || (pacman[32] = al_load_bitmap("images/pacman_powered_13.png")) == NULL
            || (pacman[33] = al_load_bitmap("images/pacman_powered_14.png")) == NULL
            || (pacman[34] = al_load_bitmap("images/pacman_powered_13.png")) == NULL
            || (pacman[35] = al_load_bitmap("images/pacman_powered_12.png")) == NULL) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while loading bitmap.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
}

void Pacman::showBody(int i, int posX, int posY, int flag) {
    al_draw_bitmap(pacman[i], posX, posY, flag);
    al_flip_display();
}

void Pacman::animateBody(int posX, int posY, int dirX, int dirY, int mode) {
    if (mode == DEFAULT || mode == SAFE) {
        if (dirX == 0 && dirY == 0) {
            showBody(i, posX, posY, CURRENT_FLAG);
            }
            if (dirY == -1) {
                CURRENT_FLAG = 0;
                showBody(i + 6, posX, posY, CURRENT_FLAG);
            }
            if (dirY == 1) {
                CURRENT_FLAG = ALLEGRO_FLIP_VERTICAL;
                showBody(i + 6, posX, posY, CURRENT_FLAG);
            }
            if (dirX == 1) {
                CURRENT_FLAG = 0;
                showBody(i, posX, posY, CURRENT_FLAG);
            }
            if (dirX == -1) {
                CURRENT_FLAG = ALLEGRO_FLIP_HORIZONTAL;
                showBody(i, posX, posY, CURRENT_FLAG);
            }
        }
        else if (mode == POWER) {
            if (time(0) % 2 == 0) {
                if (dirX == 0 && dirY == 0) {
                    showBody(i + 12, posX, posY, CURRENT_FLAG);
                }
                if (dirY == -1) {
                    CURRENT_FLAG = 0;
                    showBody(i + 18, posX, posY, CURRENT_FLAG);
                }
                if (dirY == 1) {
                    CURRENT_FLAG = ALLEGRO_FLIP_VERTICAL;
                    showBody(i + 18, posX, posY, CURRENT_FLAG);
                }
                if (dirX == 1) {
                    CURRENT_FLAG = 0;
                    showBody(i + 12, posX, posY, CURRENT_FLAG);
                }
                if (dirX == -1) {
                    CURRENT_FLAG = ALLEGRO_FLIP_HORIZONTAL;
                    showBody(i + 12, posX, posY, CURRENT_FLAG);
                }
            }
            else {
                if (dirX == 0 && dirY == 0) {
                    showBody(i + 24, posX, posY, CURRENT_FLAG);
                }
                if (dirY == -1) {
                    CURRENT_FLAG = 0;
                    showBody(i + 30, posX, posY, CURRENT_FLAG);
                }
                if (dirY == 1) {
                    CURRENT_FLAG = ALLEGRO_FLIP_VERTICAL;
                    showBody(i + 30, posX, posY, CURRENT_FLAG);
                }
                if (dirX == 1) {
                    CURRENT_FLAG = 0;
                    showBody(i + 24, posX, posY, CURRENT_FLAG);
                }
                if (dirX == -1) {
                    CURRENT_FLAG = ALLEGRO_FLIP_HORIZONTAL;
                    showBody(i + 24, posX, posY, CURRENT_FLAG);
                }
            }
        }
        if (posX != prevPosX || posY != prevPosY) {
            i++;
            if (i == 6) {
                i = 0;
            }
        }
    prevPosX = posX;
    prevPosY = posY;
}