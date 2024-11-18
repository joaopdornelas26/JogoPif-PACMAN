#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_WIDTH 80
#define MAP_HEIGHT 19
#define PACMAN 'P'
#define WALL '#'
#define EMPTY ' '
#define FOOD '.'
#define SCORES_FILE "scores.txt"

char map[MAP_HEIGHT][MAP_WIDTH + 1] = {
    "                                                                                ",
    "                                                                                ",
    "  ############################################################################# ",
    " #   #           #   #                  #    #                         #       #",
    " #   #   #########   #   ################    #######   #############   #       #",
    " #   #   #           #   #                           #   #       #      #      #",
    " #   #   #   #########   #   #########################   #   #   #######       #",
    " #   #       #           #   #       #                     #   #               #",
    " #   ###### ##############   ######  ########################   ##########     #",
    " #           #                                                                 #",
    " #   ###############   ########   ################   ######   ##########       #",
    " #   #              #   #     #   #              #   #    #   #                #",
    " #   ######   #######   ##### #   #   #######     #   #    #   #######   ####  #",
    " #   #        #             #     #   #     #     #   #             #   #      #",
    " #   #   ######   ###############     #     #########   #############   ####   #",
    " #   #   #              #                   #                          #       #",
    " #   #####   ##############   ###############   ################   #####       #",
    " #          #                   #                   #                          #",
    "  ############################################################################# "
};

int pacman_x = 39;
int pacman_y = 9;
int food_count = 0;

// Função para espalhar comidinhas por todo o mapa
void scatter_food() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == EMPTY) {
                map[y][x] = FOOD;
            }
        }
    }
    map[pacman_y][pacman_x] = PACMAN;
}

// Função para desenhar o mapa
void draw_map() {
    system("clear");
    for (int i = 0; i < MAP_HEIGHT; i++) {
        for (int j = 0; j < MAP_WIDTH; j++) {
            screenGotoxy(j, i);
            printf("%c", map[i][j]);
        }
    }
}

// Função para exibir a pontuação
void display_score() {
    screenGotoxy(0, MAP_HEIGHT);
    printf("Score: %d", food_count);
}

// Função para mover o Pacman e comer as comidinhas
void move_pacman(int new_x, int new_y) {
    if (map[new_y][new_x] == EMPTY || map[new_y][new_x] == FOOD) {
        if (map[new_y][new_x] == FOOD) {
            food_count++;
            printf("\a"); // Emite um som ao comer
        }
        map[pacman_y][pacman_x] = EMPTY;
        pacman_x = new_x;
        pacman_y = new_y;
        map[pacman_y][pacman_x] = PACMAN;
        draw_map();
        display_score(); // Atualiza a pontuação na tela
    }
}

int compare_scores(const void *a, const void *b) {
    return (*(int*)b - *(int*)a);
}

void save_score(int score) {
    FILE *file = fopen(SCORES_FILE, "a");
    if (file) {
        fprintf(file, "%d\n", score);
        fclose(file);
    }
}

void show_top_scores() {
    int scores[100], count = 0;
    FILE *file = fopen(SCORES_FILE, "r");
    if (file) {
        while (fscanf(file, "%d", &scores[count]) != EOF && count < 100) {
            count++;
        }
        fclose(file);
    }
    qsort(scores, count, sizeof(int), compare_scores);

    printf("\nTop 5 Scores:\n");
    for (int i = 0; i < count && i < 5; i++) {
        printf("%d. %d\n", i + 1, scores[i]);
    }
}

int main() {
    int key = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    scatter_food();
    draw_map();
    display_score();
    screenUpdate();

    while (key != 27) {
        if (keyhit()) { 
            key = readch();
            switch (key) {
                case 'w': move_pacman(pacman_x, pacman_y - 1); break;
                case 's': move_pacman(pacman_x, pacman_y + 1); break;
                case 'a': move_pacman(pacman_x - 1, pacman_y); break;
                case 'd': move_pacman(pacman_x + 1, pacman_y); break;
            }
            screenUpdate();
        }
    }

    save_score(food_count);
    show_top_scores();

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}