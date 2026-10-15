#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 22
#define SPEED 0.15
#define ROT_SPEED 0.10
#define STATUS_MESSAGE_LENGTH 96

typedef struct GameState {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
    int keys;
    bool has_won;
    char status_message[STATUS_MESSAGE_LENGTH];
} GameState;

void cleanup(void);
void enable_escape_char(void);
char readoutputlast(void);
void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const GameState *game);
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const int map[MAP_HEIGHT][MAP_WIDTH], double posX, double posY, double dirX, double dirY, double planeX, double planeY);
void overlay_minimap(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const int map[MAP_HEIGHT][MAP_WIDTH], const GameState *game);
bool try_move(GameState *game, int map[MAP_HEIGHT][MAP_WIDTH], double newX, double newY);

#endif