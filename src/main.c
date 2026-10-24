#include <stdio.h>
#include <string.h>

#define INVENTORY_SIZE 10
#define MAX_NAME_LEN 32

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

const char* get_item_name(int id) {
    switch (id) {
        case 0: return "Пусто";
        case 1: return "Дерево";
        case 2: return "Камень";
        case 3: return "Семена";
        case 4: return "Морковь";
        case 5: return "Пшеница";
        case 6: return "Яблоко";
        case 7: return "Яйцо";
        case 8: return "Шерсть";
        case 9: return "Молоко";
        default: return "Неизвестный предмет";
    }
}

void get_farmer_name(char *name, int max_len) {
    printf("Введите имя фермера: ");
    if (fgets(name, max_len, stdin) != NULL) {
        size_t len = strlen(name);
        if (len > 0 && name[len - 1] == '\n') {
            name[len - 1] = '\0';
        }
    }
    if (strlen(name) == 0) {
        strcpy(name, "Фермер");
    }
}

void print_menu(const char *farmer_name) {
    printf("\n========================================\n");
    printf("        ХОЗЯЙСТВО: %s\n", farmer_name);
    printf("========================================\n");
    printf("[0] Выход\n");
    printf("[1] Посмотреть на часы\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Задание лаб 1 (Устранение дубликатов)\n");
    printf("----------------------------------------\n");
    printf("Выберите пункт меню: ");
}

void print_time(int day, int hour) {
    printf("\n----------------------------------------\n");
    printf("Текущее время: День %d, %02d:00\n", day, hour);
    printf("----------------------------------------\n");
}

void calculate_work_time(int *day, int *hour, int hours_to_work) {
    int total_hours = *hour + hours_to_work;
    *day += total_hours / 24;
    *hour = total_hours % 24;
}

void handle_work(int *day, int *hour) {
    int hours_to_work;
    printf("Сколько часов вы хотите потратить на работу? ");
    if (scanf("%d", &hours_to_work) != 1) {
        printf("\n----------------------------------------\n");
        printf("Ошибка: введено не число!\n");
        printf("----------------------------------------\n");
        clear_input_buffer();
        return;
    }

    if (hours_to_work < 0) {
        printf("\n----------------------------------------\n");
        printf("Количество часов не может быть отрицательным!\n");
        printf("----------------------------------------\n");
        return;
    }

    calculate_work_time(day, hour, hours_to_work);
    print_time(*day, *hour);
}

void print_inventory(const int inventory[], int size) {
    printf("\n----------------------------------------\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
        }
    }
    printf("----------------------------------------\n");
}

void handle_put_item(int inventory[], int size) {
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
    printf("В слот %d помещен предмет [%d] (%s).\n", slot, item_id, get_item_name(item_id));
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

void handle_duplicates(int inventory[], int size) {
    printf("\n----------------------------------------\n");
    printf("Инвентарь ДО устранения дубликатов:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
        }
    }

    int removed = eliminate_duplicates(inventory, size);

    printf("\nИнвентарь ПОСЛЕ устранения дубликатов:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("Слот %d: [0]\n", i);
        } else {
            printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
        }
    }

    if (removed > 0) {
        printf("\nДубликаты успешно устранены! Очищено слотов: %d\n", removed);
    } else {
        printf("\nДубликатов не обнаружено.\n");
    }
    printf("----------------------------------------\n");
}

int main(void) {
    char farmer_name[MAX_NAME_LEN];
    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = {1, 2, 1, 3, 0, 2, 4, 1, 5, 4};
    int is_running = 1;
    int choice;

    get_farmer_name(farmer_name, MAX_NAME_LEN);

    while (is_running) {
        print_menu(farmer_name);

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
                printf("----------------------------------------\n");
                is_running = 0;
                break;

            case 1:
                print_time(current_day, current_hour);
                break;

            case 2:
                handle_work(&current_day, &current_hour);
                break;

            case 3:
                print_inventory(inventory, INVENTORY_SIZE);
                break;

            case 4:
                handle_put_item(inventory, INVENTORY_SIZE);
                break;

            case 5:
                handle_clear_slot(inventory, INVENTORY_SIZE);
                break;

            case 6:
                handle_duplicates(inventory, INVENTORY_SIZE);
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
