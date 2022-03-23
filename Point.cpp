#include "Point.h"

std::thread t2;
ALLEGRO_BITMAP* point = NULL;
ALLEGRO_BITMAP* fruit = NULL;

int pointsPositions[21][19] = { {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                 {0, 2, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 2, 0},
                                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0},
                                 {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
                                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                 {0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0},
                                 {0, 2, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 2, 0},
                                 {0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0},
                                 {0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
                                 {0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0},
                                 {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
                                 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} };

void Point::loadPointsSprites() {
    if ((point = al_load_bitmap("images/point.png")) == NULL
        || (fruit = al_load_bitmap("images/fruit.png")) == NULL) {
        al_show_native_message_box(NULL, "Error!", "An error occurred while loading bitmap.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
    }
}

void Point::generatePoints() {
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 21; j++) {
            if (pointsPositions[j][i] == 1) {
                al_draw_bitmap(point, CHUNK_SIZE * i + 15, CHUNK_SIZE * j + 15, 0);
            }
            else if (pointsPositions[j][i] == 2) {
                al_draw_bitmap(fruit, CHUNK_SIZE * i + 12, CHUNK_SIZE * j + 12, 0);
            }
        }
    }
    al_flip_display();
}

void Point::gen() {
    t2 = std::thread(&Point::generatePoints, this);
    t2.join();
}

void Point::removePoint(int posY, int posX) {
    pointsPositions[posY][posX] = 0;
}
