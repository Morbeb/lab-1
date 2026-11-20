#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* get_profession_name(Profession prof) {
    switch (prof) {
        case PROF_FARMER:
            return "Фермер";
        case PROF_LUMBERJACK:
            return "Лесоруб";
        case PROF_MINER:
            return "Шахтёр";
        case PROF_SCOUT:
            return "Разведчик";
        default:
            return "Неизвестно";
    }
}

const char* get_profession_desc(Profession prof) {
    switch (prof) {
        case PROF_FARMER:
            return "Сбалансированный труженик (HP: 100, Стамина: 50, Золото: 20)";
        case PROF_LUMBERJACK:
            return "Крепкий лесоруб (HP: 110, Стамина: 60, Золото: 15)";
        case PROF_MINER:
            return "Закалённый шахтёр (HP: 130, Стамина: 45, Золото: 30) - повышенное здоровье";
        case PROF_SCOUT:
            return "Быстрый разведчик (HP: 90, Стамина: 70, Золото: 10) - 50% шанс бесплатного шага";
        default:
            return "Обычный житель";
    }
}

Player* create_player(const char *name, Profession prof) {
    Player *player = (Player*)malloc(sizeof(Player));
    if (player == NULL) {
        printf("Ошибка выделения памяти под структуру Player!\n");
        return NULL;
    }

    strncpy(player->name, name, MAX_NAME_LEN - 1);
    player->name[MAX_NAME_LEN - 1] = '\0';
    player->profession = prof;

    switch (prof) {
        case PROF_FARMER:
            player->hp = 100;
            player->max_hp = 100;
            player->stamina = 50;
            player->max_stamina = 50;
            player->gold = 20;
            break;
        case PROF_LUMBERJACK:
            player->hp = 110;
            player->max_hp = 110;
            player->stamina = 60;
            player->max_stamina = 60;
            player->gold = 15;
            break;
        case PROF_MINER:
            player->hp = 130;
            player->max_hp = 130;
            player->stamina = 45;
            player->max_stamina = 45;
            player->gold = 30;
            break;
        case PROF_SCOUT:
            player->hp = 90;
            player->max_hp = 90;
            player->stamina = 70;
            player->max_stamina = 70;
            player->gold = 10;
            break;
        default:
            player->hp = 100;
            player->max_hp = 100;
            player->stamina = 50;
            player->max_stamina = 50;
            player->gold = 20;
            break;
    }

    player->x = 0;
    player->y = 0;

    int default_inv[INVENTORY_SIZE] = {1, 2, 1, 3, 0, 2, 4, 1, 5, 4};
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        player->inventory[i] = default_inv[i];
    }

    return player;
}

void free_player(Player **player_ptr) {
    if (player_ptr != NULL && *player_ptr != NULL) {
        printf("Память структуры по адресу %p освобождена\n", (void*)*player_ptr);
        free(*player_ptr);
        *player_ptr = NULL;
    }
}

void print_player_card(const Player *player, int current_day, int current_hour, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (player == NULL) {
        printf("\n----------------------------------------\n");
        printf("Персонаж ещё не создан! Выберите пункт [1] в меню.\n");
        printf("----------------------------------------\n");
        return;
    }

    printf("\n========================================\n");
    printf("           КАРТОЧКА ПЕРСОНАЖА           \n");
    printf("========================================\n");
    printf("Имя:         %s\n", player->name);
    printf("Профессия:   %s\n", get_profession_name(player->profession));
    printf("Особенность: %s\n", get_profession_desc(player->profession));
    printf("Здоровье:    %d / %d HP\n", player->hp, player->max_hp);
    printf("Энергия:     %d / %d\n", player->stamina, player->max_stamina);
    printf("Золото:      %d монет\n", player->gold);
    printf("Мировое время: День %d, %02d:00\n", current_day, current_hour);
    printf("----------------------------------------\n");
    printf("Инвентарь (%d слотов):\n", INVENTORY_SIZE);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (player->inventory[i] == 0) {
            printf("  [%d] Пусто\n", i);
        } else {
            printf("  [%d] [%d] %s\n", i, player->inventory[i], get_item_name(player->inventory[i], item_names));
        }
    }
    printf("========================================\n");
}

Player* handle_create_or_recreate_player(Player *current_player) {
    if (current_player != NULL) {
        printf("\n----------------------------------------\n");
        printf("Внимание: персонаж '%s' уже существует!\n", current_player->name);
        printf("Точно хотите удалить его и создать заново? (1 - Да, 0 - Отмена): ");
        int confirm = 0;
        if (scanf("%d", &confirm) != 1 || confirm != 1) {
            printf("Пересоздание отменено.\n");
            printf("----------------------------------------\n");
            clear_input_buffer();
            return current_player;
        }
        clear_input_buffer();
        free_player(&current_player);
    } else {
        clear_input_buffer();
    }

    char name_buf[MAX_NAME_LEN];
    printf("\nВведите имя персонажа: ");
    if (fgets(name_buf, sizeof(name_buf), stdin) != NULL) {
        size_t len = strlen(name_buf);
        if (len > 0 && name_buf[len - 1] == '\n') {
            name_buf[len - 1] = '\0';
        }
    }
    if (strlen(name_buf) == 0) {
        strcpy(name_buf, "Герой");
    }

    int prof_choice = 0;
    while (prof_choice < 1 || prof_choice > 4) {
        printf("\nВыберите класс персонажа:\n");
        printf("[1] Фермер    - Сбалансированный труженик (HP: 100, Стамина: 50, Золото: 20)\n");
        printf("[2] Лесоруб   - Крепкий дровосек        (HP: 110, Стамина: 60, Золото: 15)\n");
        printf("[3] Шахтёр    - Закалённый горняк       (HP: 130, Стамина: 45, Золото: 30)\n");
        printf("[4] Разведчик - Ловкий следопыт         (HP: 90,  Стамина: 70, Золото: 10, 50%% бесплатный шаг)\n");
        printf("Ваш выбор (1-4): ");
        if (scanf("%d", &prof_choice) != 1) {
            printf("Ошибка ввода! Введите число от 1 до 4.\n");
            clear_input_buffer();
            prof_choice = 0;
            continue;
        }
        if (prof_choice < 1 || prof_choice > 4) {
            printf("Неверный номер класса! Повторите ввод.\n");
        }
    }
    clear_input_buffer();

    Player *new_player = create_player(name_buf, (Profession)prof_choice);
    if (new_player != NULL) {
        printf("\n----------------------------------------\n");
        printf("Персонаж '%s' класса '%s' успешно создан!\n", new_player->name, get_profession_name(new_player->profession));
        printf("----------------------------------------\n");
    }
    return new_player;
}
