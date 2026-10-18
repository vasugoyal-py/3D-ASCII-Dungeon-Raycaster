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

void present(char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1], int color[SCREEN_HEIGHT][SCREEN_WIDTH])

{
    static char frame[128*1024];
    int pos = 0;

    int ret = sprintf(frame + pos, "\033[H");
    if (ret != -1)
    {
        pos += ret;
    }
    int ccolor = -1;

    for(int c = 0; c < SCREEN_HEIGHT; c++)
    {
        for(int r = 0; r < SCREEN_WIDTH; r++)
        {
            int cellcol = color[c][r];
            if (cellcol != ccolor)
            {
                ccolor = cellcol;
                switch(ccolor)
                {
                    case COLOR_KEY:
                        ret = sprintf(frame + pos, "\033[93m");
                        if (ret != -1)
                        {
                            pos += ret;
                        }
                        break;

                    case COLOR_DOOR:
                        ret = sprintf(frame + pos, "\033[38;5;130m");
                        if (ret != -1)
                        {
                            pos += ret;
                        }
                        break;

                    case COLOR_PORTAL:
                        ret = sprintf(frame + pos, "\033[95m");
                        if (ret != -1)
                        {
                            pos += ret;
                        }
                        break;

                    case COLOR_WALL:
                        ret = sprintf(frame + pos, "\033[37m");
                        if (ret != -1)
                        {
                            pos += ret;
                        }
                        break;

                    case COLOR_DEFAULT:
                    default:
                        ret = sprintf(frame + pos, "\033[0m");
                        if (ret != -1)
                        {
                            pos += ret;
                        }
                        break;
                }   
            }

            char ch = screen[c][r];
            switch (ch)
            {
                case '1':
                    ret = sprintf(frame + pos, "█"); 
                    if (ret != -1)
                    {
                        pos += ret;
                    }
                    break;
                case '2': 
                    ret = sprintf(frame + pos, "▓"); 
                    if (ret != -1)
                    {
                        pos += ret;
                    }
                    break;
                case '3': 
                    ret = sprintf(frame + pos, "▒");
                    if (ret != -1)
                    {
                        pos += ret;
                    }
                    break;
                case '4':
                    ret = sprintf(frame + pos, "░");
                    if (ret != -1)
                    {
                        pos += ret;
                    }
                    break;
                default:
                    frame[pos++] = ch; 
                    break;
            }
        }

        frame[pos++] = '\n';
    
    }

    ret = sprintf(frame + pos, "\033[0m");
    if (ret != -1)
    {
        pos += ret; 
    }
    frame[pos] = '\0';
    fputs(frame, stdout);
}


void render(
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1],
    int color[SCREEN_HEIGHT][SCREEN_WIDTH],
    const Map *map,
    double posX, double posY,
    double dirX, double dirY,
    double planeX, double planeY)
{

    for (int i = 0; i < SCREEN_HEIGHT; i++)
    {
        screen[i][SCREEN_WIDTH] = '\0';
    }
    
    for(int c = 0; c < SCREEN_WIDTH; c++)
    {
        // calc ray pos and direc
        float cameraxX = (2 * c / (double)SCREEN_WIDTH) - 1.0;
        double rayDirX = dirX + (cameraxX * planeX);
        double rayDirY = dirY + (cameraxX * planeY);
        
        int mapX = (int)posX;
        int mapY = (int)posY;

        // Digital Differential Analysis set

        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1.0 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1.0 / rayDirY);

        int stepX, stepY;
        double sideX, sideY;

        if (rayDirX < 0) 
        {
            stepX = -1;
            sideX = (posX - mapX) * deltaDistX;
        }
        else 
        {
            stepX = 1;
            sideX = (mapX + 1.0 - posX) * deltaDistX;
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideY = (posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideY = (mapY + 1.0 - posY) * deltaDistY;
        }
        // init dda
        int hit = 0;
        int side = 0;
        int hitTile= 0;
    
        while (!hit)
        {
            if (sideX < sideY) 
            {
                sideX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else 
            {
                sideY += deltaDistY;
                mapY += stepY;
                side = 1;
            }
            if (mapX < 0 || mapX >= map->width || mapY < 0 || mapY >= map->height)
            {
                hit = 1;
                break;
            }

            int tile = map->data[(mapY * map->width) + mapX];
            if (tile > 0)
            {
                hit = 1;
                hitTile = tile;
            }
        }  

        // wall dist form player
        double wallDist;
        if (side == 0) 
        {
            wallDist = (sideX - deltaDistX);
        }
        else
        {
            wallDist = (sideY - deltaDistY);
        }

        if (wallDist < 0.0001)
        {
            wallDist = 0.0001;
        }

        // length of all

        int lineHeight =  (int)(SCREEN_HEIGHT / wallDist);

        int drawStart = -(lineHeight / 2) + (SCREEN_HEIGHT / 2);
        if (drawStart < 0)
        {
            drawStart = 0;
        }

        int drawEnd = (lineHeight / 2) + (SCREEN_HEIGHT / 2);
        if (drawEnd >= SCREEN_HEIGHT)
        {
            drawEnd = SCREEN_HEIGHT - 1;
        }

        // what is there
        char wallChar;
        int wallColo = COLOR_WALL;

        if (hitTile == 2) 
        {
            // Locked Door
            wallChar = (side == 1) ? ':' : '|';
            wallColo = COLOR_DOOR;
        }
        else if (hitTile == 3) 
        {
            // Key
            wallChar = '*';
            wallColo = COLOR_KEY;
        }
        else if (hitTile == 9) 
        {
            // Exit Goal
            wallChar = '@';
            wallColo = COLOR_PORTAL;
        }
        else
        {
            if (wallDist < 2.0)
            {
                wallChar = '1'; // █
            }
            else if (wallDist < 4.0)
            {   
                wallChar = '2'; // ▓
            }
            else if (wallDist < 8.0) 
            {
                wallChar = '3'; // ▒
            }
            else
            {
                wallChar = '4'; // ░
            }

            if (side == 1) {
                if (wallChar == '1') wallChar = '2';
                else if (wallChar == '2') wallChar = '3';
                else if (wallChar == '3') wallChar = '4';
            }
            wallColo = COLOR_WALL;
        }
        // make col
        for (int r = 0; r < SCREEN_HEIGHT; r++) {
            if (r < drawStart)
            {
                screen[r][c] = ' ';
                color[r][c] = COLOR_DEFAULT;
            } 
            else if(r <= drawEnd)
            {
                screen[r][c] = wallChar;
                color[r][c] = wallColo;
            }
            else
            {
                screen[r][c] = '.';
                color[r][c] = COLOR_DEFAULT;
            }
        }
    }
}