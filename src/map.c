#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Map* create_map(int width, int height, int floor) {
    if (width <= 0 || height <= 0) {
        return NULL;
    }

    Map *map = (Map*)malloc(sizeof(Map));
    if (map == NULL) {
        printf("Ошибка выделения памяти под структуру Map!\n");
        return NULL;
    }

    map->width = width;
    map->height = height;
    map->current_floor = floor;
    map->start_x = 0;
    map->start_y = 0;
    map->up_x = 0;
    map->up_y = 0;

    map->grid = (char**)malloc(height * sizeof(char*));
    if (map->grid == NULL) {
        printf("Ошибка выделения памяти под строки карты!\n");
        free(map);
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        map->grid[i] = (char*)malloc(width * sizeof(char));
        if (map->grid[i] == NULL) {
            printf("Ошибка выделения памяти под ячейки карты!\n");
            for (int j = 0; j < i; j++) {
                free(map->grid[j]);
            }
            free(map->grid);
            free(map);
            return NULL;
        }
    }

    return map;
}

void free_map(Map **map_ptr) {
    if (map_ptr != NULL && *map_ptr != NULL) {
        Map *map = *map_ptr;
        if (map->grid != NULL) {
            for (int i = 0; i < map->height; i++) {
                if (map->grid[i] != NULL) {
                    free(map->grid[i]);
                }
            }
            free(map->grid);
        }
        printf("Память структуры по адресу %p освобождена\n", (void*)map);
        free(map);
        *map_ptr = NULL;
    }
}

void generate_tower_floor(Map *map) {
    if (map == NULL || map->grid == NULL) {
        return;
    }

    int h = map->height;
    int w = map->width;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i == 0 || i == h - 1 || j == 0 || j == w - 1) {
                map->grid[i][j] = '#';
            } else {
                int r = rand() % 100;
                if (r < OBSTACLE_CHANCE / 2) {
                    map->grid[i][j] = 'T';
                } else if (r < OBSTACLE_CHANCE) {
                    map->grid[i][j] = 'R';
                } else {
                    map->grid[i][j] = '.';
                }
            }
        }
    }

    int sx = 1 + rand() % (w - 2);
    int sy = 1 + rand() % (h - 2);
    map->grid[sy][sx] = 'D';
    map->start_x = sx;
    map->start_y = sy;

    int ux, uy;
    do {
        ux = 1 + rand() % (w - 2);
        uy = 1 + rand() % (h - 2);
    } while (ux == sx && uy == sy);

    map->grid[uy][ux] = 'U';
    map->up_x = ux;
    map->up_y = uy;

    int rune_count = (w * h) / 35;
    if (rune_count < 2) rune_count = 2;
    if (rune_count > 5) rune_count = 5;

    for (int k = 0; k < rune_count; k++) {
        int rx, ry, attempts = 0;
        do {
            rx = 1 + rand() % (w - 2);
            ry = 1 + rand() % (h - 2);
            attempts++;
        } while (map->grid[ry][rx] != '.' && attempts < 100);

        if (map->grid[ry][rx] == '.') {
            map->grid[ry][rx] = '^';
        }
    }
}

void print_map(const Map *map, int player_x, int player_y) {
    if (map == NULL || map->grid == NULL) {
        return;
    }

    printf("\n========================================\n");
    printf("        БАШНЯ: ЭТАЖ %d (%d x %d)\n", map->current_floor, map->width, map->height);
    printf("========================================\n");

    for (int i = 0; i < map->height; i++) {
        for (int j = 0; j < map->width; j++) {
            if (i == player_y && j == player_x) {
                printf("@");
            } else {
                printf("%c", map->grid[i][j]);
            }
        }
        printf("\n");
    }

    printf("----------------------------------------\n");
    printf("Обозначения:\n");
    printf("  @ - Герой             # - Стена башни\n");
    printf("  . - Пол               T, R - Препятствия\n");
    printf("  D - Лестница вниз     U - Лестница вверх\n");
    printf("  ^ - Руна телепортации (ловушка)\n");
    printf("----------------------------------------\n");
}

int is_valid_coord(const Map *map, int x, int y) {
    if (map == NULL) return 0;
    return (x >= 0 && x < map->width && y >= 0 && y < map->height);
}

void trigger_rune_trap(Map *map, int *player_x, int *player_y) {
    if (map == NULL || player_x == NULL || player_y == NULL) {
        return;
    }

    map->grid[*player_y][*player_x] = '.';

    int tx = *player_x;
    int ty = *player_y;
    int attempts = 0;
    do {
        int rx = 1 + rand() % (map->width - 2);
        int ry = 1 + rand() % (map->height - 2);
        if (map->grid[ry][rx] == '.') {
            tx = rx;
            ty = ry;
            break;
        }
        attempts++;
    } while (attempts < 200);

    *player_x = tx;
    *player_y = ty;

    printf("\n========================================\n");
    printf("ВСПЫШКА МАГИИ! Вы наступили на руну (^)\n");
    printf("Телепортационная ловушка перенесла вас в другое место этажа!\n");
    printf("========================================\n");
}
