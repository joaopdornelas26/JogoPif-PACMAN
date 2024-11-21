#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x;
    int y;
    char symbol;
    int score;
} Pacman;

#define MAP_WIDTH 80
#define MAP_HEIGHT 19
#define PACMAN_SYMBOL 'P'
#define WALL '#'
#define EMPTY ' '
#define FOOD '.'
#define SCORES_FILE "scores.txt"

char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "                                                                                ",
    "--------------------------------------PACMAN------------------------------------",
    " ###############################################################################",
    " #   #           #   #                  #    #                         #       #",
    " #   #   #########   #   ################    #######   #############   #       #",
    " #   #   #           #   #                           #   #       #      #      #",
    " #   #   #   #########   #   #########################   #   #   #######       #",
    " #   #       #           #   #       #                     #   #               #",
    " #   ####   ##############   ######  ########################   ##########     #",
    " #           #                                                                 #",
    " #   ###############   ########   ################   ######   ##########       #",
    " #   #              #   #         #              #   #    #   #                #",
    " #   ######   #######   ##### #   #   #######     #   #    #   #######   ####  #",
    " #   #                      #     #   #     #     #   #             #   #      #",
    " #   #   ######   ###############     #     #########   #############   ####   #",
    " #   #   #              #                   #                          #       #",
    " #   #####   ##############   ###############   ################   #####       #",
    " #          #                   #                   #                          #",
    " ###############################################################################"
};

void scatter_food(Pacman *pacman) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == EMPTY) {
                map[y][x] = FOOD;
            }
        }
    }
    map[pacman->y][pacman->x] = pacman->symbol;
}

void draw_map() {
    system("clear");
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            screenGotoxy(j, i);
            printf("%c", map[i][j]);
        }
    }
}

void display_score(int score) {
    screenGotoxy(0, MAP_HEIGHT);
    printf("Score: %d", score);
}

void move_pacman(Pacman *pacman, int new_x, int new_y) {
    if (map[new_y][new_x] == EMPTY || map[new_y][new_x] == FOOD) {
        if (map[new_y][new_x] == FOOD) {
            pacman->score++;
            printf("\a"); 
        }
        map[pacman->y][pacman->x] = EMPTY;
        pacman->x = new_x;
        pacman->y = new_y;
        map[pacman->y][pacman->x] = pacman->symbol;
        draw_map();
        display_score(pacman->score);
    }
}


void save_score(int score) {
    FILE *file = fopen(SCORES_FILE, "a");
    if (file) {
        fprintf(file, "%d\n", score);
        fclose(file);
    }
}


int main() {
    int key = 0;

    Pacman pacman = {39, 9, PACMAN_SYMBOL, 0}; 

    screenInit(1);
    keyboardInit();
    timerInit(50);

    scatter_food(&pacman); 
    draw_map();            
    display_score(pacman.score); 
    screenUpdate();

    
    while (key != 27) { 
        if (keyhit()) { 
            key = readch();
            switch (key) {
                case 'w': move_pacman(&pacman, pacman.x, pacman.y - 1); break;
                case 's': move_pacman(&pacman, pacman.x, pacman.y + 1); break;
                case 'a': move_pacman(&pacman, pacman.x - 1, pacman.y); break;
                case 'd': move_pacman(&pacman, pacman.x + 1, pacman.y); break;
            }
            screenUpdate();
        }
    }

    save_score(pacman.score); 
    
    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
