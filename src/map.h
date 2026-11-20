#ifndef MAP_H
#define MAP_H

#define MIN_TOWER_SIZE 5
#define MAX_TOWER_SIZE 30
#define OBSTACLE_CHANCE 20

typedef struct Map {
    char **grid;
    int width;
    int height;
    int current_floor;
    int start_x;
    int start_y;
    int up_x;
    int up_y;
} Map;

Map* create_map(int width, int height, int floor);
void free_map(Map **map_ptr);
void generate_tower_floor(Map *map);
void print_map(const Map *map, int player_x, int player_y);
int is_valid_coord(const Map *map, int x, int y);
void trigger_rune_trap(Map *map, int *player_x, int *player_y);

#endif // MAP_H
