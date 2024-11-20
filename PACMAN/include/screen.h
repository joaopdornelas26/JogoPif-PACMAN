#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdio.h>


#define MAP_WIDTH 80
#define MAP_HEIGHT 19


extern char map[MAP_HEIGHT][MAP_WIDTH + 1];


void screenClear();
void screenInit(int drawBorders);
void screenDestroy();
void screenGotoxy(int x, int y);
void screenUpdate();


void screenDrawMap();

#endif 
