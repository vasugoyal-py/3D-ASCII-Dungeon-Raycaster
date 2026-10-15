/*
// This is the code of helper.h
#ifndef HELPER_H
#define HELPER_H

#include <stdbool.h>
#include "map.h"

#define SCREEN_WIDTH 150
#define SCREEN_HEIGHT 40
#define speed 0.15
#define rotSpeed 0.10
#define loglen 500
typedef struct GameState {
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
} GameState;

void cleanup(void);
void enable_escape_char(void);
char readoutputlast(void);
void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]);
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], const Map *map, double posX, double posY, double dirX, double dirY, double planeX, double planeY);
bool can_move(const Map *map, double newX, double newY);

#endif
*/

//this is helper.c
#include "helper.h"

#include <stdio.h>
#include <windows.h>
#include <math.h>

void cleanup(void)
{
    printf("\033[?25h");
}

void enable_escape_char(void)
{
    SetConsoleOutputCP(CP_UTF8);

HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    DWORD mode;
    GetConsoleMode(hOut, &mode);

    SetConsoleMode(
        hOut,
        mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
    );
}

char readoutputlast(void)
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hOut, &info);

    COORD cursor = info.dwCursorPosition;

    // nothing typed yet on this line
    if (cursor.X == 0)
        return '\0';

    // read the single character immediately before the cursor
    COORD pos = { cursor.X - 1, cursor.Y };

    char ch;
    DWORD charsRead;
    ReadConsoleOutputCharacter(hOut, &ch, 1, pos, &charsRead);

    return ch;
}

void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], int color[SCREEN_HEIGHT][SCREEN_WIDTH])
{
    static char frame[128 * 1024];
    int pos = 0;

    pos += sprintf(frame + pos, "\033[H");

    int current_color = -1; // current color

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            int cellColor = color[y][x];

            if (cellColor != current_color)
            {
                current_color = cellColor;
                switch (current_color)
                {
                    case COLOR_KEY:
                        pos += sprintf(frame + pos, "\033[93m"); // Bright Yellow
                        break;
                    case COLOR_DOOR:
                        pos += sprintf(frame + pos, "\033[38;5;130m"); // Wood Brown
                        break;
                    case COLOR_PORTAL:
                        pos += sprintf(frame + pos, "\033[95m"); // Bright Purple / Magenta
                        break;
                    case COLOR_WALL:
                        pos += sprintf(frame + pos, "\033[37m"); // Wall Gray / White
                        break;
                    case COLOR_DEFAULT:
                    default:
                        pos += sprintf(frame + pos, "\033[0m");  // Reset to default
                        break;
                }
            }

            char ch = screen[y][x];
            switch (ch)
            {
                case '1': pos += sprintf(frame + pos, "█"); break;
                case '2': pos += sprintf(frame + pos, "▓"); break;
                case '3': pos += sprintf(frame + pos, "▒"); break;
                case '4': pos += sprintf(frame + pos, "░"); break;
                default:  frame[pos++] = ch; break;
            }
        }
        frame[pos++] = '\n';
    }

    pos += sprintf(frame + pos, "\033[0m");
    frame[pos] = '\0';

    fputs(frame, stdout);
}

bool can_move(const Map *map, double newX, double newY)
{
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= map->width || mapY < 0 || mapY >= map->height)
    {
        return false;
    }

    return map->data[(mapY * map->width) + mapX] == 0;
}

bool try_move(GameState *game, double newX, double newY)
{
    int mapX = (int)newX;
    int mapY = (int)newY;

    if (mapX < 0 || mapX >= game->map->width || mapY < 0 || mapY >= game->map->height)
    {
        return false;
    }

    int index = (mapY * game->map->width) + mapX;
    int tile = game->map->data[index];

    switch (tile)
    {
    case 0: // Empty
        return true;
        break;
    case 1: // Wall
        return false;
        break;
    case 3: // Key
        game->key_count++;
        snprintf(game->log, sizeof(game->log), "Found a Key! (Keys: %d/%d)", game->key_count, game->total_key);
        game->map->data[index] = 0;
        return true;
        break;
    case 2: // locked door
        if (game->key_count > 0)
        {
            game->key_count--;
            game->map->data[index] = 0;
            snprintf(game->log, sizeof(game->log), "Unlocked door! (Keys left: %d/%d)", game->key_count, game->total_key);
            return true;
            break;
        }
        else
        {
            snprintf(game->log, sizeof(game->log), "The door is locked! You need a key.");
            return false;
            break;
        }
    case 9: // win?
        game->won = true;
        snprintf(game->log, sizeof(game->log), "You reached the exit! You win!");
        return true;
    default:
        return false;
        break;
    }
}

// old render function 
/*
void render(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], int map[MAP_HEIGHT][MAP_WIDTH], double posX, double posY, double dirX, double dirY, double planeX, double planeY)
{
    (void)planeX;
    (void)planeY;

    for (int y = 0; y < SCREEN_HEIGHT; y++)
    {
        for (int x = 0; x < SCREEN_WIDTH; x++)
        {
            screen[y][x] = ' ';
        }
        screen[y][SCREEN_WIDTH] = '\0'; 
    }

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            screen[y][x] = map[y][x] ? '#' : '.';
        }
    }

    int playerScreenX = (int)posX;
    int playerScreenY = (int)posY;
    screen[playerScreenY][playerScreenX] = 'P';

    int arrowX = playerScreenX;
    int arrowY = playerScreenY;
    char arrow = '^';

    if (fabs(dirX) > fabs(dirY))
    {
        arrowX += dirX > 0 ? 1 : -1;
        arrow = dirX > 0 ? '>' : '<';
    }
    else
    {
        arrowY += dirY > 0 ? 1 : -1;
        arrow = dirY > 0 ? 'v' : '^';
    }

    if (arrowX >= 0 && arrowX < SCREEN_WIDTH && arrowY >= 0 && arrowY < SCREEN_HEIGHT)
    {
        screen[arrowY][arrowX] = arrow;
    }
}
*/

void render(
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
    int color[SCREEN_HEIGHT][SCREEN_WIDTH],
    const Map *map,
    double posX, double posY,
    double dirX, double dirY,
    double planeX, double planeY)
{
    // Step A: Set null terminators for every row
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        screen[y][SCREEN_WIDTH] = '\0';
    }

    // Step B: Cast a ray for each vertical column
    for (int x = 0; x < SCREEN_WIDTH; x++)
    {
        // 1. Calculate ray position and direction
        double cameraX = 2.0 * x / (double)SCREEN_WIDTH - 1.0; // -1 (left) .. 1 (right)
        double rayDirX = dirX + planeX * cameraX;
        double rayDirY = dirY + planeY * cameraX;

        int mapX = (int)posX;
        int mapY = (int)posY;

        // 2. Set up DDA step parameters
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);

        int stepX, stepY;
        double sideDistX, sideDistY;

        if (rayDirX < 0) {
            stepX = -1;
            sideDistX = (posX - mapX) * deltaDistX;
        } else {
            stepX = 1;
            sideDistX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0) {
            stepY = -1;
            sideDistY = (posY - mapY) * deltaDistY;
        } else {
            stepY = 1;
            sideDistY = (mapY + 1.0 - posY) * deltaDistY;
        }

        // 3. Perform DDA grid traversal
        int hit = 0;
        int side = 0; 
        int hitTile = 0;

        while (!hit) {
            if (sideDistX < sideDistY) {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            } else {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= map->width || mapY < 0 || mapY >= map->height) {
                hit = 1; 
                break;
            }

            int tile = map->data[mapY * map->width + mapX];
            if (tile > 0) {
                hit = 1;
                hitTile = tile;
            }
        }

        // 4. Calculate perpendicular distance
        double perpWallDist;
        if (side == 0) {
            perpWallDist = (sideDistX - deltaDistX);
        } else {
            perpWallDist = (sideDistY - deltaDistY);
        }
        if (perpWallDist < 0.0001) perpWallDist = 0.0001;

        // 5. Calculate wall column height and draw boundaries
        int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);

        int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;

        int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
        if (drawEnd >= SCREEN_HEIGHT) drawEnd = SCREEN_HEIGHT - 1;

        // 6. Select character based on what was hit
        char wallChar;
        int wallColor = COLOR_WALL;

        if (hitTile == 2) {
            // Locked Door
            wallChar = (side == 1) ? ':' : '|';
            wallColor = COLOR_DOOR;
        } else if (hitTile == 3) {
            // Key
            wallChar = '*';
            wallColor = COLOR_KEY;
        } else if (hitTile == 9) {
            // Exit Goal
            wallChar = '@';
            wallColor = COLOR_PORTAL;
        } else {
            // Normal Wall: Distance-based shading tokens
            if (perpWallDist < 2.0)      wallChar = '1'; // █
            else if (perpWallDist < 4.0) wallChar = '2'; // ▓
            else if (perpWallDist < 8.0) wallChar = '3'; // ▒
            else                          wallChar = '4'; // ░

            // Shadow on Y-side walls for 3D depth
            if (side == 1) {
                if (wallChar == '1') wallChar = '2';
                else if (wallChar == '2') wallChar = '3';
                else if (wallChar == '3') wallChar = '4';
            }
            wallColor = COLOR_WALL;
        }

        // Step 7: Fill screen and color buffers
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            if (y < drawStart) {
                screen[y][x] = ' ';
                color[y][x] = COLOR_DEFAULT;
            } else if (y <= drawEnd) {
                screen[y][x] = wallChar;
                color[y][x] = wallColor;
            } else {
                screen[y][x] = '.';
                color[y][x] = COLOR_DEFAULT;
            }
        }
    }
}