//This is main.c

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <string.h>

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
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 1, 3, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
        {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
        {1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 1, 2, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
        {1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 1, 1, 0, 1, 1, 1, 1, 1, 2, 1, 0, 1, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 9, 1},
        {1, 0, 1, 1, 1, 1, 1, 3, 1, 1, 1, 0, 0, 0, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    GameState game = {
        .posX = 1.5,
        .posY = 1.5,
        .dirX = 1.0,
        .dirY = 0.0,
        .planeX = 0.0,
        .planeY = 0.66,
        .keys = 0,
        .has_won = false,
        .status_message = "Find keys, unlock doors, and reach the exit."
    };

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; 
    
    int running = 1;
    system("cls");

    while (running != 0 && !game.has_won)
{
    if (kbhit())
    {
        u_char c = getch(); // never change this u_char to any other data types it bricks the program idk why but it does so never change it
        if (c == 'Q' || c == 'q')
        {
            strcpy(game.status_message, "Exited the dungeon.");
            running = 0;
        }
        if (c == 'w' || c == 'W')
        {
            double newY = game.posY + (game.dirY * SPEED);
            double newX = game.posX + (game.dirX * SPEED);

            try_move(&game, map, newX, game.posY);
            try_move(&game, map, game.posX, newY);
        }
        if (c == 's' || c == 'S')
        {
            double newY = game.posY - (game.dirY * SPEED);
            double newX = game.posX - (game.dirX * SPEED);

            try_move(&game, map, newX, game.posY);
            try_move(&game, map, game.posX, newY);
        }
        if (c == 'a' || c == 'A')
        {
            double oldX = game.dirX;
            game.dirX = game.dirX * cos(ROT_SPEED) + game.dirY * sin(ROT_SPEED);
            game.dirY = -oldX * sin(ROT_SPEED) + game.dirY * cos(ROT_SPEED);

            double oldPlaneX = game.planeX;
            game.planeX = game.planeX * cos(ROT_SPEED) + game.planeY * sin(ROT_SPEED);
            game.planeY = -oldPlaneX * sin(ROT_SPEED) + game.planeY * cos(ROT_SPEED);
        }
        if (c == 'd' || c == 'D')
        {

            double oldX = game.dirX;
            game.dirX = game.dirX * cos(ROT_SPEED) - game.dirY * sin(ROT_SPEED);
            game.dirY = oldX * sin(ROT_SPEED) + game.dirY * cos(ROT_SPEED);

            double oldPlaneX = game.planeX;
            game.planeX = game.planeX * cos(ROT_SPEED) - game.planeY * sin(ROT_SPEED);
            game.planeY = oldPlaneX * sin(ROT_SPEED) + game.planeY * cos(ROT_SPEED);
            
        }
    }

    render(screen, map, game.posX, game.posY, game.dirX, game.dirY, game.planeX, game.planeY);
    overlay_minimap(screen, map, &game);
    present(screen, &game);

    Sleep(33);
}

    if (game.has_won)
    {
        system("cls");
        printf("\n\n");
        printf("========================================\n");
        printf("            VICTORY! YOU ESCAPED        \n");
        printf("========================================\n");
        printf("Keys used: %d\n", game.keys);
        printf("%s\n", game.status_message);
        printf("\nPress any key to exit...");
        getch();
    }

    return 0;
}
