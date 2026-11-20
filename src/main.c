#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "player.h"

#define MIN_TOWER_SIZE 5
#define MAX_TOWER_SIZE 30
#define OBSTACLE_CHANCE 20

void print_menu(const Player *player) {
    printf("\n========================================\n");
    if (player != NULL) {
        printf("    ХОЗЯЙСТВО: %s [%s]\n", player->name, get_profession_name(player->profession));
    } else {
        printf("    ХОЗЯЙСТВО: Персонаж не создан\n");
    }
    printf("========================================\n");
    printf("[0] Выход\n");
    printf("[1] Создать / пересоздать персонажа\n");
    printf("[2] Карточка персонажа\n");
    printf("[3] Режим дня и управление временем\n");
    printf("[4] Посмотреть инвентарь\n");
    printf("[5] Положить предмет в слот\n");
    printf("[6] Выбросить предмет\n");
    printf("[7] Задание лаб 1 (Устранение дубликатов)\n");
    printf("[8] Поиск предмета по названию\n");
    printf("[9] Записать состояние в дневник (diary.txt)\n");
    printf("[10] Распаковать карту участка (output.txt)\n");
    printf("[11] Отправиться в приключение (Башня)\n");
    printf("----------------------------------------\n");
    printf("Выберите пункт меню: ");
}

int check_player_exists(const Player *player) {
    if (player == NULL) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: персонаж ещё не создан! Выберите пункт [1] в меню.\n");
        printf("----------------------------------------\n");
        return 0;
    }
    return 1;
}

void print_inventory(const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("\n----------------------------------------\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i], item_names));
        }
    }
    printf("----------------------------------------\n");
}

void handle_put_item(int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    int slot;
    int item_id;

    printf("Введите индекс слота (от 0 до %d): ", size - 1);
    if (scanf("%d", &slot) != 1) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: введено не число!\n");
        printf("----------------------------------------\n");
        clear_input_buffer();
        return;
    }

    if (slot < 0 || slot >= size) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: индекс выходит за границы массива (0..%d)!\n", size - 1);
        printf("----------------------------------------\n");
        return;
    }

    printf("Введите ID предмета (от 0 до 9): ");
    if (scanf("%d", &item_id) != 1) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: введено не число!\n");
        printf("----------------------------------------\n");
        clear_input_buffer();
        return;
    }

    if (item_id < 0 || item_id > 9) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: ID предмета должен быть от 0 до 9!\n");
        printf("----------------------------------------\n");
        return;
    }

    inventory[slot] = item_id;
    printf("\n----------------------------------------\n");
    printf("В слот %d помещен предмет [%d] (%s).\n", slot, item_id, get_item_name(item_id, item_names));
    printf("----------------------------------------\n");
}

void handle_clear_slot(int inventory[], int size) {
    int slot;
    printf("Введите индекс слота для очистки (от 0 до %d): ", size - 1);
    if (scanf("%d", &slot) != 1) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: введено не число!\n");
        printf("----------------------------------------\n");
        clear_input_buffer();
        return;
    }

    if (slot < 0 || slot >= size) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: индекс выходит за границы массива (0..%d)!\n", size - 1);
        printf("----------------------------------------\n");
        return;
    }

    inventory[slot] = 0;
    printf("\n----------------------------------------\n");
    printf("Слот %d очищен.\n", slot);
    printf("----------------------------------------\n");
}

int eliminate_duplicates(int inventory[], int size) {
    int duplicates_removed = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] != 0) {
            for (int j = i + 1; j < size; j++) {
                if (inventory[j] == inventory[i]) {
                    inventory[j] = 0;
                    duplicates_removed++;
                }
            }
        }
    }
    return duplicates_removed;
}

void handle_duplicates(int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("\n----------------------------------------\n");
    printf("Инвентарь ДО устранения дубликатов:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i], item_names));
        }
    }

    int removed = eliminate_duplicates(inventory, size);

    printf("\nИнвентарь ПОСЛЕ устранения дубликатов:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i], item_names));
        }
    }

    if (removed > 0) {
        printf("\nДубликаты успешно устранены! Очищено слотов: %d\n", removed);
    } else {
        printf("\nДубликатов не обнаружено.\n");
    }
    printf("----------------------------------------\n");
}

void handle_search_item(const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    char search_name[MAX_NAME_LEN];
    printf("Введите название предмета для поиска: ");
    clear_input_buffer();
    if (fgets(search_name, MAX_NAME_LEN, stdin) != NULL) {
        size_t len = strlen(search_name);
        if (len > 0 && search_name[len - 1] == '\n') {
            search_name[len - 1] = '\0';
        }
    }

    if (strlen(search_name) == 0) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: пустое название!\n");
        printf("----------------------------------------\n");
        return;
    }

    int item_id = find_item_by_name(item_names, MAX_ITEMS, search_name);
    if (item_id == -1) {
        printf("\n----------------------------------------\n");
        printf("Предмет \"%s\" не найден в каталоге!\n", search_name);
        printf("----------------------------------------\n");
        return;
    }

    printf("\n----------------------------------------\n");
    printf("Результаты поиска для \"%s\" (ID: %d):\n", search_name, item_id);
    int found_count = 0;
    for (int i = 0; i < size; i++) {
        if (inventory[i] == item_id) {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i], item_names));
            found_count++;
        }
    }

    if (found_count == 0) {
        printf("Предмет отсутствует в инвентаре.\n");
    } else {
        printf("Всего найдено в инвентаре: %d шт.\n", found_count);
    }
    printf("----------------------------------------\n");
}

char** create_map(int height, int width) {
    char **map = (char **)malloc(height * sizeof(char *));
    if (map == NULL) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        map[i] = (char *)malloc(width * sizeof(char));
        if (map[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(map[j]);
            }
            free(map);
            return NULL;
        }
    }

    return map;
}

void free_map(char **map, int height) {
    if (map == NULL) {
        return;
    }

    for (int i = 0; i < height; i++) {
        if (map[i] != NULL) {
            free(map[i]);
        }
    }
    free(map);
}

void generate_tower_floor(char **map, int height, int width, int *player_x, int *player_y, int *up_x, int *up_y) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                map[i][j] = '#';
            } else {
                int r = rand() % 100;
                if (r < OBSTACLE_CHANCE / 2) {
                    map[i][j] = 'T';
                } else if (r < OBSTACLE_CHANCE) {
                    map[i][j] = 'R';
                } else {
                    map[i][j] = '.';
                }
            }
        }
    }

    int sx = 1 + rand() % (width - 2);
    int sy = 1 + rand() % (height - 2);
    map[sy][sx] = 'D';
    *player_x = sx;
    *player_y = sy;

    int ux, uy;
    do {
        ux = 1 + rand() % (width - 2);
        uy = 1 + rand() % (height - 2);
    } while (ux == sx && uy == sy);

    map[uy][ux] = 'U';
    *up_x = ux;
    *up_y = uy;

    int rune_count = (width * height) / 35;
    if (rune_count < 2) {
        rune_count = 2;
    }
    if (rune_count > 5) {
        rune_count = 5;
    }

    for (int k = 0; k < rune_count; k++) {
        int rx, ry, attempts = 0;
        do {
            rx = 1 + rand() % (width - 2);
            ry = 1 + rand() % (height - 2);
            attempts++;
        } while (map[ry][rx] != '.' && attempts < 100);

        if (map[ry][rx] == '.') {
            map[ry][rx] = '^';
        }
    }
}

void print_map(char **map, int height, int width, int player_x, int player_y, int floor_num) {
    printf("\n========================================\n");
    printf("        БАШНЯ: ЭТАЖ %d (%d x %d)\n", floor_num, width, height);
    printf("========================================\n");

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == player_y && j == player_x) {
                printf("@");
            } else {
                printf("%c", map[i][j]);
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

void handle_adventure(Player *player) {
    if (!check_player_exists(player)) {
        return;
    }

    if (player->stamina <= 0) {
        printf("\n----------------------------------------\n");
        printf("У вас нет сил для похода (Выносливость: 0)! Отдохните и выспитесь.\n");
        printf("----------------------------------------\n");
        return;
    }

    int height = MIN_TOWER_SIZE + rand() % (MAX_TOWER_SIZE - MIN_TOWER_SIZE + 1);
    int width = MIN_TOWER_SIZE + rand() % (MAX_TOWER_SIZE - MIN_TOWER_SIZE + 1);

    char **map = create_map(height, width);
    if (map == NULL) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: не удалось выделить память под карту башни!\n");
        printf("----------------------------------------\n");
        return;
    }

    int player_x, player_y, up_x, up_y;
    generate_tower_floor(map, height, width, &player_x, &player_y, &up_x, &up_y);
    int current_floor = 1;
    int in_tower = 1;

    printf("\nВы (%s, класс %s) вошли в древнюю башню!\n", player->name, get_profession_name(player->profession));

    while (in_tower) {
        print_map(map, height, width, player_x, player_y, current_floor);

        printf("Управление: [W] вверх, [A] влево, [S] вниз, [D] вправо, [Q] сбежать домой\n");
        printf("Ваш ход: ");

        char input[16];
        if (scanf("%15s", input) != 1) {
            clear_input_buffer();
            continue;
        }

        char move = input[0];
        int dx = 0;
        int dy = 0;

        if (move == 'w' || move == 'W') {
            dy = -1;
        } else if (move == 's' || move == 'S') {
            dy = 1;
        } else if (move == 'a' || move == 'A') {
            dx = -1;
        } else if (move == 'd' || move == 'D') {
            dx = 1;
        } else if (move == 'q' || move == 'Q') {
            if (map[player_y][player_x] == 'D') {
                printf("\n========================================\n");
                printf("Вы стоите на лестнице вниз и сбегаете домой на ферму!\n");
                printf("Итог экспедиции: покорено этажей башни — %d.\n", current_floor);
                printf("========================================\n");
                in_tower = 0;
                break;
            } else {
                printf("\n----------------------------------------\n");
                printf("Сбежать домой нельзя! Выход (Q) возможен только с лестницы вниз (D).\n");
                printf("----------------------------------------\n");
                continue;
            }
        } else {
            printf("\n----------------------------------------\n");
            printf("Неизвестная команда! Используйте W, A, S, D или Q.\n");
            printf("----------------------------------------\n");
            continue;
        }

        int target_x = player_x + dx;
        int target_y = player_y + dy;

        if (target_x < 0 || target_x >= width || target_y < 0 || target_y >= height) {
            printf("\n----------------------------------------\n");
            printf("Вы не можете выйти за границы карты!\n");
            printf("----------------------------------------\n");
            continue;
        }

        char cell = map[target_y][target_x];
        if (cell == '#') {
            printf("\n----------------------------------------\n");
            printf("Там каменная стена! Прохода нет.\n");
            printf("----------------------------------------\n");
            continue;
        } else if (cell == 'T' || cell == 'R') {
            printf("\n----------------------------------------\n");
            printf("Путь преграждает препятствие (%s)!\n", cell == 'T' ? "обломок балки" : "груда камней");
            printf("----------------------------------------\n");
            continue;
        } else if (cell == '^') {
            map[target_y][target_x] = '.';

            int tx = target_x;
            int ty = target_y;
            int attempts = 0;
            do {
                int rx = 1 + rand() % (width - 2);
                int ry = 1 + rand() % (height - 2);
                if (map[ry][rx] == '.') {
                    tx = rx;
                    ty = ry;
                    break;
                }
                attempts++;
            } while (attempts < 200);

            player_x = tx;
            player_y = ty;

            printf("\n========================================\n");
            printf("ВСПЫШКА МАГИИ! Вы наступили на руну (^)\n");
            printf("Телепортационная ловушка перенесла вас в другое место этажа!\n");
            printf("========================================\n");
            continue;
        }

        player_x = target_x;
        player_y = target_y;

        if (map[player_y][player_x] == 'U') {
            printf("\n----------------------------------------\n");
            printf("Вы ступили на лестницу вверх (U)!\n");
            printf("Подъем на следующий этаж: %d -> %d...\n", current_floor, current_floor + 1);
            printf("----------------------------------------\n");

            free_map(map, height);
            current_floor++;

            height = MIN_TOWER_SIZE + rand() % (MAX_TOWER_SIZE - MIN_TOWER_SIZE + 1);
            width = MIN_TOWER_SIZE + rand() % (MAX_TOWER_SIZE - MIN_TOWER_SIZE + 1);
            map = create_map(height, width);
            if (map == NULL) {
                printf("Ошибка выделения памяти для этажа %d!\n", current_floor);
                return;
            }

            generate_tower_floor(map, height, width, &player_x, &player_y, &up_x, &up_y);
        } else if (map[player_y][player_x] == 'D') {
            printf("\nВы стоите на лестнице вниз (D). Отсюда можно сбежать домой (Q).\n");
        }
    }

    free_map(map, height);
}

int main(void) {
    srand((unsigned int)time(NULL));
    Player *player = NULL;
    char item_names[MAX_ITEMS][MAX_NAME_LEN];
    int current_day = 1;
    int current_hour = 8;
    int is_running = 1;
    int choice;

    if (!load_items(item_names)) {
        printf("Предупреждение: файл items.txt не найден! Загружены базовые названия.\n");
    }

    while (is_running) {
        print_menu(player);

        if (scanf("%d", &choice) != 1) {
            printf("\n----------------------------------------\n");
            printf("Ошибка: необходимо ввести целое число!\n");
            printf("----------------------------------------\n");
            clear_input_buffer();
            continue;
        }

        switch (choice) {
            case 0:
                printf("\n----------------------------------------\n");
                printf("Завершение работы программы.\n");
                if (player != NULL) {
                    free_player(&player);
                }
                printf("----------------------------------------\n");
                is_running = 0;
                break;

            case 1:
                player = handle_create_or_recreate_player(player);
                break;

            case 2:
                print_player_card(player, current_day, current_hour, item_names);
                break;

            case 3: {
                int prev_day = current_day;
                handle_time_menu(&current_day, &current_hour);
                if (current_day > prev_day && player != NULL) {
                    player->hp = player->max_hp;
                    player->stamina = player->max_stamina;
                    printf("\nВы отлично выспались! Здоровье (%d HP) и выносливость (%d) полностью восстановлены.\n", player->hp, player->stamina);
                }
                break;
            }

            case 4:
                if (check_player_exists(player)) {
                    print_inventory(player->inventory, INVENTORY_SIZE, item_names);
                }
                break;

            case 5:
                if (check_player_exists(player)) {
                    handle_put_item(player->inventory, INVENTORY_SIZE, item_names);
                }
                break;

            case 6:
                if (check_player_exists(player)) {
                    handle_clear_slot(player->inventory, INVENTORY_SIZE);
                }
                break;

            case 7:
                if (check_player_exists(player)) {
                    handle_duplicates(player->inventory, INVENTORY_SIZE, item_names);
                }
                break;

            case 8:
                if (check_player_exists(player)) {
                    handle_search_item(player->inventory, INVENTORY_SIZE, item_names);
                }
                break;

            case 9:
                if (check_player_exists(player)) {
                    handle_save_diary(player->name, current_day, current_hour, player->inventory, INVENTORY_SIZE, item_names);
                }
                break;

            case 10:
                handle_unpack_map();
                break;

            case 11:
                handle_adventure(player);
                break;

            default:
                printf("\n----------------------------------------\n");
                printf("Неверный пункт меню!\n");
                printf("----------------------------------------\n");
                break;
        }
    }

    return 0;
}
