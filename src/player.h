#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

typedef enum {
    PROF_FARMER = 1,
    PROF_LUMBERJACK = 2,
    PROF_MINER = 3,
    PROF_SCOUT = 4
} Profession;

typedef struct Player {
    char name[MAX_NAME_LEN];
    Profession profession;
    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;
    int x;
    int y;
    int inventory[INVENTORY_SIZE];
} Player;

Player* create_player(const char *name, Profession prof);
void free_player(Player **player_ptr);
const char* get_profession_name(Profession prof);
const char* get_profession_desc(Profession prof);
void print_player_card(const Player *player, int current_day, int current_hour, const char item_names[MAX_ITEMS][MAX_NAME_LEN]);
Player* handle_create_or_recreate_player(Player *current_player);

#endif // PLAYER_H
