#include "Map.h"

int mapGrid[672][608];
enum objects { NOTHING, WALL, PLAYER, GHOST1, GHOST2, GHOST3 };
ALLEGRO_BITMAP* background = NULL;

void Map::loadChunks() {
    for (int i = 0; i < 21; i++) {
        for (int j = 0; j < 19; j++) {
            if ((i % CHUNK_SIZE == 0) && (j % CHUNK_SIZE == 0)) {
                if (mapChunks[i / CHUNK_SIZE][j / CHUNK_SIZE] == 1) {
                    for (int l = i; l < i + CHUNK_SIZE; l++) {
                        for (int k = j; k < j + CHUNK_SIZE; k++) {
                            mapGrid[l][k] = 1;
                        }
                    }
                }
            }
        }
    }
}

void Map::loadBackground() {
    if ((background = al_load_bitmap("images/map.png")) == NULL) {
        al_show_native_message_box(NULL, "Error!", "An error occurred while loading bitmap.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
    drawBackground();
    al_flip_display();
}

void Map::drawBackground() {
    al_draw_bitmap(background, 0, 0, 0);
}