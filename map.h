// this is code of map.h

#ifndef MAP_H
#define MAP_H

typedef struct Map
{
    const char *name; // name of map

    int width; // len adn width
    int height;

    const int *data; // the dimentions of map selected

    double startX; // starting position of player in each map
    double startY;

    double dirX; // Player direction
    double dirY;

    double planeX; // Camera direction
    double planeY;


}Map;

int map_count(void);

const Map* get_map(int index);

#endif