#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <thread>
#include <string>
#include <time.h>
#include <Windows.h>
#include "Pacman.h"
#include "Map.h"
#include "Point.h"
#include "Ghost.h"

using namespace std;

#define TICK 0.04

class Engine : public Map, public Pacman, public Point {
private:
    enum directions { UP, DOWN, LEFT, RIGHT, STAY };
    enum playerModes { DEFAULT, POWER, SAFE };
    unsigned int resX = 0, resY = 0;
    unsigned char r = 0, g = 0, b = 0;
    int startButtonX = 160, startButtonY = 270;
    int exitButtonX = 160, exitButtonY = 350;
    int startButtonWidth = 310, startButtonHeight = 50;
    int exitButtonWidth = 310, exitButtonHeight = 50;
    bool startButtonActive = false;
    bool exitButtonActive = false;
    int direction = 3;
    bool KEYBOARD_ON = false;
    bool MOUSE_ON = false;
    bool inGame = false;
    bool firstMove = true;
    bool gameStarted = false;
    bool gamePaused = false;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_DISPLAY_MODE displayMode;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    ALLEGRO_BITMAP* heart = NULL;
    ALLEGRO_EVENT ev;
    ALLEGRO_FONT* defaultFont;
    ALLEGRO_FONT* bigFont;
    time_t startTime;
    time_t safeTime;
    int prevTime;

protected:
    int playerPosX = 9;  // * CHUNK_SIZE + CHUNK_SIZE / 2;
    int playerPosY = 15; // * CHUNK_SIZE + CHUNK_SIZE / 2;
    int chunkPosX = 2, chunkPosY = 2;
    int playerScoreCounter = 0;
    int playerLifesCounter = 3;
    int playerMode = DEFAULT;
    char playerScore[4];
    char playerLifes[2];
    Ghost* ghosts[4];

public:
    // allegro library initialization
    void init() {
        if (!al_init()) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the Allegro library.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
        if (!al_init_font_addon()) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the Allegro font addon.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
        if (!al_init_ttf_addon()) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the Allegro ttf addon.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
        if (!al_init_image_addon()) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the Allegro image addon.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
        if (!al_init_primitives_addon()) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the Allegro primitives library.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
        if ((heart = al_load_bitmap("images/heart.png")) == NULL) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while loading bitmap.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
        }
        defaultFont = al_load_ttf_font("fonts/Raleway-Light.ttf", 14, 0);
        bigFont = al_load_ttf_font("fonts/Raleway-Light.ttf", 72, 0);
    }

    // set graphic options - mode (fullscreen or windowed), resolution and background color
    void setGfxOptions(int mode, unsigned int resX, unsigned int resY, unsigned char r, unsigned char g, unsigned char b) {
        this->resX = resX;
        this->resY = resY;
        this->r = r;
        this->g = g;
        this->b = b;
        al_set_new_display_flags(mode);
    }

    // set other options - whether to initialize keyboard or mouse 
    void setOptions(bool keyboard_state, bool mouse_state, int refresh_rate) {
        this->KEYBOARD_ON = keyboard_state;
        this->MOUSE_ON = mouse_state;
        displayMode.refresh_rate = refresh_rate;
        eventQueue = al_create_event_queue();

        if (KEYBOARD_ON) {
            if (!al_install_keyboard()) {
                al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the keyboard.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
                exit(-1);
            }
            al_register_event_source(eventQueue, al_get_keyboard_event_source());
        }

        if (MOUSE_ON) {
            if (!al_install_mouse()) {
                al_show_native_message_box(NULL, "Error!", "An error occurred while initializing the mouse.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
                exit(-1);
            }
            al_register_event_source(eventQueue, al_get_mouse_event_source());
        }
    }

    // display main frame
    void displayGfx() {
        display = al_create_display(resX, resY);
        if (display == NULL) {
            al_show_native_message_box(NULL, "Error!", "An error occurred while creating the display.", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
            exit(-1);
        }
    }

    void drawHearts(int type, int playerLifesCounter) {
        if (type == 1) {
            if (playerLifesCounter > 0) {
                al_draw_bitmap(heart, 85, 694, 0);
                if (playerLifesCounter > 1) {
                    al_draw_bitmap(heart, 103, 694, 0);
                    if (playerLifesCounter > 2) {
                        al_draw_bitmap(heart, 121, 694, 0);
                    }
                }
            }
        }
        if (type == 2) {
            if (playerLifesCounter > 0) {
                al_draw_bitmap(heart, 358, 338, 0);
                if (playerLifesCounter > 1) {
                    al_draw_bitmap(heart, 376, 338, 0);
                    if (playerLifesCounter > 2) {
                        al_draw_bitmap(heart, 394, 338, 0);
                    }
                }
            }
        }
    }

    void startGame() {
        gameStarted = true;
        startScreenInfo(3);
        startScreenInfo(2);
        startScreenInfo(1);
    }

    void startScreen() {
        gameStarted = false;
        int r1 = 65, g1 = 65, b1 = 65;
        int r2 = 65, g2 = 65, b2 = 65;
        while (true) {
            setBackgroundColor(21, 20, 20);
            if (!gameStarted) {
                al_draw_text(bigFont, al_map_rgb(255, 255, 255), 314, 56, ALLEGRO_ALIGN_CENTRE, "PAC-MAN");
                al_draw_filled_rectangle(startButtonX, startButtonY, startButtonX + startButtonWidth, startButtonY + startButtonHeight, al_map_rgb(r1, g1, b1));
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 314, 286, ALLEGRO_ALIGN_CENTRE, "Rozpoczecie gry - wcisnij ENTER");
                al_draw_filled_rectangle(exitButtonX, exitButtonY, exitButtonX + exitButtonWidth, exitButtonY + exitButtonHeight, al_map_rgb(r2, g2, b2));
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 314, 366, ALLEGRO_ALIGN_CENTRE, "Wyjscie z gry - wcisnij ESCAPE");
            }
            al_flip_display();
            al_wait_for_event(eventQueue, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    startGame();
                    break;
                }
            }
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    gameStarted = false;
                    exit(0);
                    break;
                }
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_AXES) {
                if ((ev.mouse.x >= startButtonX) && (ev.mouse.x <= startButtonX + startButtonWidth) && (ev.mouse.y >= startButtonY) && (ev.mouse.y <= startButtonY + startButtonHeight)) {
                    startButtonActive = true;
                    r1 = 95; g1 = 95; b1 = 95;
                }
                else {
                    startButtonActive = false;
                    r1 = 65; g1 = 65; b1 = 65;
                }
                if ((ev.mouse.x >= exitButtonX) && (ev.mouse.x <= exitButtonX + exitButtonWidth) && (ev.mouse.y >= exitButtonY) && (ev.mouse.y <= exitButtonY + exitButtonHeight)) {
                    exitButtonActive = true;
                    r2 = 95; g2 = 95; b2 = 95;
                }
                else {
                    exitButtonActive = false;
                    r2 = 65; g2 = 65; b2 = 65;
                }
            }
            if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (startButtonActive) {
                    startGame();
                    break;
                }
                if (exitButtonActive) {
                    gameStarted = false;
                    exit(0);
                    break;
                }
            }
        }
    }
    
    void startScreenInfo(int sec) {
        char seconds[3];
        sprintf_s(seconds, "%d", sec);
        setBackgroundColor(21, 20, 20);
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 306, ALLEGRO_ALIGN_CENTRE, "Przygotuj sie!");
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 326, ALLEGRO_ALIGN_CENTRE, seconds);
        al_flip_display();
        al_rest(1);
    }

    void lostLifeInfo(int sec) {
        char seconds[3];
        sprintf_s(seconds, "%d", sec);
        setBackgroundColor(21, 20, 20);
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 306, ALLEGRO_ALIGN_CENTRE, "STRATA ZYCIA");
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 218, 336, 0, "Pozostala liczba zyc: ");
        drawHearts(2, playerLifesCounter);
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 366, ALLEGRO_ALIGN_CENTRE, "Przygotuj sie!");
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 396, ALLEGRO_ALIGN_CENTRE, seconds);
        al_flip_display();
        al_rest(1);
    }

    void gameoverScreen(bool gameWon, int playerLifesCounter) {
        while (true) {
            setBackgroundColor(21, 20, 20);
            if (gameWon) {
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 326, ALLEGRO_ALIGN_CENTRE, "ZWYCIESTWO");
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 260, 356, 0, "Twoj wynik: ");
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 338, 356, 0, playerScore);
                al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 386, ALLEGRO_ALIGN_CENTRE, "Wcisnij enter aby zakonczyc...");
                al_flip_display();
                al_wait_for_event(eventQueue, &ev);
                if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                        inGame = false;
                        exit(0);
                        break;
                    }
                }
            }
            else {
                if (playerLifesCounter > 0) {
                    gamePaused = true;
                    sprintf_s(playerLifes, "%d", playerLifesCounter);
                    lostLifeInfo(3);
                    lostLifeInfo(2);
                    lostLifeInfo(1);
                    gamePaused = false;
                    playerMode = SAFE;
                    safeTime = time(0);
                    firstMove = true;
                    break;
                }
                else {
                    al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 326, ALLEGRO_ALIGN_CENTRE, "KONIEC GRY");
                    al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 260, 356, 0, "Twoj wynik: ");
                    al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 338, 356, 0, playerScore);
                    al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 304, 386, ALLEGRO_ALIGN_CENTRE, "Wcisnij enter aby zakonczyc...");
                    al_flip_display();
                    al_wait_for_event(eventQueue, &ev);
                    if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                        if (ev.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                            inGame = false;
                            exit(0);
                            break;
                        }
                    }
                }
            }
        }
    }

    void setBackgroundColor(unsigned char r, unsigned char g, unsigned char b) {
        this->r = r;
        this->g = g;
        this->b = b;
        al_clear_to_color(al_map_rgb(r, g, b));
        al_flip_display();
    }

    // destroy main frame
    void destroy() {
        if (KEYBOARD_ON) {
            al_uninstall_keyboard();
            KEYBOARD_ON = false;
        }
        if (MOUSE_ON) {
            al_uninstall_keyboard();
            MOUSE_ON = false;
        }
        al_destroy_display(display);
    }

    void mainLoop() {
        al_unregister_event_source(eventQueue, al_get_mouse_event_source());
        generatePoints();
        sprintf_s(playerLifes, "%d", playerLifesCounter);
        showBody(0, playerPosX * CHUNK_SIZE, playerPosY * CHUNK_SIZE, 0);
        playerMove();
    }


    bool ghostCollision(Ghost *ghost) {
        if (playerPosX == ghost->positionX && playerPosY == ghost->positionY) {
            return true;
        }
        
        if (playerPosX == ghost->positionX) {
            if (playerPosY - ghost->positionY == 1) {
                if (chunkPosY < ghost->chunkPosY) return true;
            }
            if (playerPosY - ghost->positionY == -1) {
                if (chunkPosY > ghost->chunkPosY) return true;
            }
        }
        if (playerPosY == ghost->positionY) {
            if (playerPosX - ghost->positionX == 1) {
                if (chunkPosX < ghost->chunkPosX) return true;
            }
            if (playerPosX - ghost->positionX == -1) {
                if (chunkPosX > ghost->chunkPosX) return true;
            }
        }
        return false;
    }


    bool changeDirection() {
        int dirX = 0, dirY = 0;

        if (direction == UP) {
            dirX = 0;
            dirY = -1;
        }
        if (direction == DOWN) {
            dirX = 0;
            dirY = 1;
        }
        if (direction == LEFT) {
            dirX = -1;
            dirY = 0;
        }
        if (direction == RIGHT) {
            dirX = 1;
            dirY = 0;
        }
        if (chunkPosX == 0 && chunkPosY == 0) {
            if (mapChunks[playerPosY + dirY][playerPosX + dirX] == 0) {
                if (chunkPosX == 0 && chunkPosY == 0)
                    return true;
            }
        }
        else {
            if (dirX != 0 && chunkPosY == 0)
                return true;
            if (dirY != 0 && chunkPosX == 0)
                return true;
        }

        return false;

    }

    void move() {
        while (inGame) {
            al_wait_for_event(eventQueue, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                switch (ev.keyboard.keycode) {
                case ALLEGRO_KEY_UP:
                case ALLEGRO_KEY_W:
                    direction = UP;
                    break;
                case ALLEGRO_KEY_DOWN:
                case ALLEGRO_KEY_S:
                    direction = DOWN;
                    break;
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    direction = RIGHT;
                    break;
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    direction = LEFT;
                    break;
                }
            }
        }
    }

    void pacmanRest(int dirX, int dirY) {
        setBackgroundColor(21, 20, 20);
        drawBackground();
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 4, 676, 0, "Liczba punktow:");
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 120, 676, 0, playerScore);
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 4, 692, 0, "Liczba zyc:");
        drawHearts(1, playerLifesCounter);
        //al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 120, 692, 0, playerLifes);
        animateBody(playerPosX * CHUNK_SIZE + 8 * chunkPosX, playerPosY * CHUNK_SIZE + 8 * chunkPosY, dirX, dirY, playerMode);
        for (int i = 0; i < 4; i++) {
            if (ghosts[i]->alive && !gamePaused) {
                ghosts[i]->move();
                ghosts[i]->drawGhost();
            }
        }
        for (int i = 0; i < 4; i++) {
            if (ghostCollision(ghosts[i])&& ghosts[i]->alive) {
                if (playerMode == POWER || playerMode == SAFE) {
                    ghosts[i]->positionX = 9;
                    ghosts[i]->positionY = 9;
                }
                else {
                    playerPosX = 9;
                    playerPosY = 15;
                    chunkPosX = 0;
                    chunkPosY = 0;
                    playerLifesCounter--;
                    gameoverScreen(false, playerLifesCounter);
                }
            }
        }
        generatePoints();
        if (chunkPosX == 0 && chunkPosY == 0) {
            if (pointsPositions[playerPosY][playerPosX] == 1) {
                playerScoreCounter++;
                if (playerScoreCounter > 185) {
                    gameoverScreen(true, playerLifesCounter);
                }
                sprintf_s(playerScore, "%d", playerScoreCounter);
                removePoint(playerPosY, playerPosX);
            }
            else if (pointsPositions[playerPosY][playerPosX] == 2) {
                playerMode = POWER;
                startTime = time(0);
                playerScoreCounter += 10;
                if (playerScoreCounter > 185) {
                    gameoverScreen(true, playerLifesCounter);
                }
                sprintf_s(playerScore, "%d", playerScoreCounter);
                removePoint(playerPosY, playerPosX);
            }
        }
        al_rest(TICK);
    }

    void playerMove() {
        inGame = true;

        std::thread t1(&Engine::move, this);

        chunkPosX = 0;
        chunkPosY = 0;
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 4, 676, 0, "Liczba punktow:");
        al_draw_text(defaultFont, al_map_rgb(255, 255, 255), 120, 676, 0, "0");
        al_flip_display();

        for (int i = 0; i < 4; i++) {
            ghosts[i] = new Ghost(9,9);
        }
        int dirX = 0, dirY = 0;
        
        while (true) {
            if (playerMode != DEFAULT) {
                if (time(0) - startTime == 8) {
                    playerMode = DEFAULT;
                }
                if (time(0) - safeTime == 2) {
                    playerMode = DEFAULT;
                }
            }
            if (inGame) {
                if (changeDirection()) {
                    if (firstMove) {
                        direction = STAY;
                    }

                    if (direction == UP) {
                        dirX = 0;
                        dirY = -1;
                    }
                    if (direction == DOWN) {
                        dirX = 0;
                        dirY = 1;
                    }
                    if (direction == LEFT) {
                        dirX = -1;
                        dirY = 0;
                    }
                    if (direction == RIGHT) {
                        dirX = 1;
                        dirY = 0;
                    }
                    if (direction == STAY) {
                        dirX = 0;
                        dirY = 0;
                    }
                }

                firstMove = false;

                if (mapChunks[playerPosY + dirY][playerPosX + dirX] == 0 || chunkPosX != 0 || chunkPosY != 0) {
                    chunkPosX += dirX;
                    chunkPosY += dirY;
                    if (chunkPosX > 1 || chunkPosX < -2 || chunkPosY>1 || chunkPosY < -2) {

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


                        playerPosX += dirX;
                        playerPosY += dirY;
                    }
                    pacmanRest(dirX, dirY);

                    if (playerPosX == 18 || playerPosX == 0) {

                        if (dirX > 0)
                            playerPosX = 1;
                        else
                            playerPosX = 17;
                    }

                    if (playerPosY == 20 || playerPosY == 0) {
                        if (dirY > 0)
                            playerPosY = 1;
                        else
                            playerPosY = 19;
                    }
                }
                else {
                    pacmanRest(dirX, dirY);
                }
            }
            else {
                t1.join();
                al_rest(2);
                break;
            }
        }
    }
};


int main() {
    srand(time(NULL));

    Engine engine;
    engine.init();
    engine.loadChunks();
    engine.setGfxOptions(ALLEGRO_WINDOWED, 608, 712, 21, 20, 20);
    engine.setOptions(true, true, 60);
    engine.displayGfx();
    engine.setBackgroundColor(21, 20, 20);
    engine.loadPacmanSprites();
    engine.loadPointsSprites();
    engine.startScreen();
    engine.loadBackground();
    engine.mainLoop();
    engine.destroy();
    
    return 0;
}

