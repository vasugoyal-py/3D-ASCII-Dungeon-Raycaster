//This is main.c

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

#include "helper.h"

int main(void)
{
    enable_escape_char();
    // setvbuf(stdout, NULL, _IONBF, 0);
    atexit(cleanup);
    printf("\033[?25l");


    int map[MAP_HEIGHT][MAP_WIDTH] = 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    };
    
   /*
    int map[MAP_HEIGHT][MAP_WIDTH] = 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
    */
    GameState game = {
        .posX = 3.0,
        .posY = 3.0,
        .dirX = 0.0,
        .dirY = -1.0,
        .planeX = 0.66,
        .planeY = 0.0,
        .key = 0,
        .key_count = 0,
        .won = false,
        .log = " "
    };

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; 
    
    int running = 1;
    system("cls");

    while (running != 0)
{
    if (kbhit())
    {
        u_char c = getch(); // never change this u_char to any other data types it bricks the program idk why but it does so never change it
        if (c == 'Q' || c == 'q')
        {
            running = 0;
        }
        if (c == 'w' || c == 'W')
        {
            double newY = game.posY + (game.dirY * speed);
            double newX = game.posX + (game.dirX * speed);

            if (can_move(map, newX, game.posY)) game.posX = newX;
            if (can_move(map, game.posX, newY)) game.posY = newY;
        }
        if (c == 's' || c == 'S')
        {
            double newY = game.posY - (game.dirY * speed);
            double newX = game.posX - (game.dirX * speed);

            if (can_move(map, newX, game.posY)) game.posX = newX;
            if (can_move(map, game.posX, newY)) game.posY = newY;
        }
        if (c == 'a' || c == 'A')
        {
            double oldX = game.dirX;
            game.dirX = game.dirX * cos(rotSpeed) + game.dirY * sin(rotSpeed);
            game.dirY = -oldX * sin(rotSpeed) + game.dirY * cos(rotSpeed);

            double oldPlaneX = game.planeX;
            game.planeX = game.planeX * cos(rotSpeed) + game.planeY * sin(rotSpeed);
            game.planeY = -oldPlaneX * sin(rotSpeed) + game.planeY * cos(rotSpeed);
        }
        if (c == 'd' || c == 'D')
        {

            double oldX = game.dirX;
            game.dirX = game.dirX * cos(rotSpeed) - game.dirY * sin(rotSpeed);
            game.dirY = oldX * sin(rotSpeed) + game.dirY * cos(rotSpeed);

            double oldPlaneX = game.planeX;
            game.planeX = game.planeX * cos(rotSpeed) - game.planeY * sin(rotSpeed);
            game.planeY = oldPlaneX * sin(rotSpeed) + game.planeY * cos(rotSpeed);
            
        }
    }

    render(screen, map, game.posX, game.posY, game.dirX, game.dirY, game.planeX, game.planeY);
    present(screen);

    Sleep(33);
}
    

    return 0;
}
