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

    char screen[SCREEN_HEIGHT][SCREEN_WIDTH + 1]; 
    
    int running = 1;
    system("cls");

    printf("Select a map:\n");
    int no_of_map = map_count(); // called outside the for loop so the for loop does't call it over and over leading to inefficency
    for(int i = 0; i < no_of_map; i++)
    {
        const Map *m = get_map(i);
        printf("%d). %s (%d * %d)\n", i + 1, m->name, m->width, m->height);
    }
    printf("Enter: ");
    int choise = 0;
    scanf("%d", &choise);
    choise -= 1;
    if (choise < 0 || choise >= no_of_map)
    {
        return 1;
    }
    const Map *select_map = get_map(choise);

    GameState game = 
    {
        .posX = select_map->startX,
        .posY = select_map->startY,
        .dirX = select_map->dirX,
        .dirY = select_map->dirY,
        .planeX = select_map->planeX,
        .planeY = select_map->planeY,
        .map = select_map,
        .total_key = select_map->total_key,
        .key_count = 0,
        .won = false,
        .log = " "
    };

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

                if (try_move(&game, newX, game.posY)) game.posX = newX;
                if (try_move(&game, game.posX, newY)) game.posY = newY;
            }
            if (c == 's' || c == 'S')
            {
                double newY = game.posY - (game.dirY * speed);
                double newX = game.posX - (game.dirX * speed);

                if (try_move(&game, newX, game.posY)) game.posX = newX;
                if (try_move(&game, game.posX, newY)) game.posY = newY;
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

        if (game.won)  
        {
            render(screen, game.map, game.posX, game.posY, game.dirX, game.dirY, game.planeX, game.planeY);
            present(screen);
            system("cls");
            printf("\n*** %s ***\n", game.log);
            break;
        }

        render(screen, game.map, game.posX, game.posY, game.dirX, game.dirY, game.planeX, game.planeY);
        present(screen);
        printf("Keys: %d / %d\n", game.key_count, game.total_key);
        printf("%s\n", game.log);

        Sleep(33);
    }
    
    return 0;
}
