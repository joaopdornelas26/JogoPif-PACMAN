#include "screen.h"
#include "keyboard.h"
#include "timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PACMAN 'P'
#define FOOD '.'
#define EMPTY ' '
#define GHOST 'G'
#define NUM_GHOSTS 3

void saveScore(int score);

int pacman_x = 39, pacman_y = 9, food_count = 0;
int ghost_positions[NUM_GHOSTS][2] = {
    {10, 10},
    {20, 5},
    {30, 15}
};

void scatter_food() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == EMPTY) {
                map[y][x] = FOOD;
            }
        }
    }
    map[pacman_y][pacman_x] = PACMAN;
    for (int i = 0; i < NUM_GHOSTS; i++) {
        map[ghost_positions[i][1]][ghost_positions[i][0]] = GHOST;
    }
}

void move_pacman(int new_x, int new_y) {
    if (map[new_y][new_x] == '#') return;
    if (map[new_y][new_x] == GHOST) {
        printf("\nVocê foi pego por um fantasma! Fim de jogo.\n");
        keyboardDestroy();
        screenDestroy();
        saveScore(food_count);
        exit(0);
    }
    if (map[new_y][new_x] == FOOD) food_count++;
    map[pacman_y][pacman_x] = EMPTY;
    pacman_x = new_x;
    pacman_y = new_y;
    map[pacman_y][pacman_x] = PACMAN;
    screenDrawMap();
    printf("\nScore: %d\n", food_count);
}

void move_ghosts() {
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    for (int i = 0; i < NUM_GHOSTS; i++) {
        map[ghost_positions[i][1]][ghost_positions[i][0]] = EMPTY;
        int direction = rand() % 4;
        int new_x = ghost_positions[i][0] + dx[direction];
        int new_y = ghost_positions[i][1] + dy[direction];
        if (map[new_y][new_x] != '#' && map[new_y][new_x] != PACMAN && map[new_y][new_x] != GHOST) {
            ghost_positions[i][0] = new_x;
            ghost_positions[i][1] = new_y;
        }
        map[ghost_positions[i][1]][ghost_positions[i][0]] = GHOST;
        if (ghost_positions[i][0] == pacman_x && ghost_positions[i][1] == pacman_y) {
            printf("\nVocê foi pego por um fantasma! Fim de jogo.\n");
            keyboardDestroy();
            screenDestroy();
            saveScore(food_count);
            exit(0);
        }
    }
}

void saveScore(int score) {
    FILE *file = fopen("score.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo para salvar o score.\n");
        return;
    }
    fprintf(file, "Score: %d\n", score);
    fclose(file);
    printf("Score salvo em 'score.txt'!\n");
}

int main() {
    screenInit(0);
    keyboardInit();
    scatter_food();
    screenDrawMap();
    srand(time(NULL));
    int ch;
    while ((ch = readch()) != 27) {
        switch (ch) {
            case 'w': move_pacman(pacman_x, pacman_y - 1); break;
            case 's': move_pacman(pacman_x, pacman_y + 1); break;
            case 'a': move_pacman(pacman_x - 1, pacman_y); break;
            case 'd': move_pacman(pacman_x + 1, pacman_y); break;
        }
        move_ghosts();
        screenDrawMap();
    }
    saveScore(food_count);
    keyboardDestroy();
    screenDestroy();
    return 0;
}
