// This is the code of helper.h
#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>
#include "map.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 24
#define speed 0.15
#define rotSpeed 0.10
#define loglen 500
typedef struct GameState{
    double posX; // Player pos
    double posY;

    double dirX; // Player direction
    double dirY;

    double planeX; // Camera direction
    double planeY;

    int key; // if key
    int key_count; // no of keys

    bool won; // has won?

    char log[loglen]; // the characters displayed to the screen

    const Map *map;
}GameState;

void cleanup(void);
void enable_escape_char(void);
char readoutputlast(void);
void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const Map *map, double posX, double posY, double dirX, double dirY, double planeX, double planeY);
bool can_move(const Map *map, double newX, double newY);

#endif