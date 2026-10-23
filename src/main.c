#include <stdio.h>

#define INVENTORY_SIZE 10

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

int main(void) {
    int current_day = 1;
    int current_hour = 8;
    int inventory[INVENTORY_SIZE] = {1, 2, 1, 3, 0, 2, 4, 1, 5, 4};
    int is_running = 1;
    int choice;

    while (is_running) {
        printf("\n========================================\n");
        printf("              МЕНЮ ФЕРМЫ\n");
        printf("========================================\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("[3] Посмотреть инвентарь\n");
        printf("[4] Положить предмет в слот\n");
        printf("[5] Выбросить предмет\n");
        printf("----------------------------------------\n");
        printf("Выберите пункт меню: ");

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
                printf("\n----------------------------------------\n");
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                printf("----------------------------------------\n");
                break;

            case 2: {
                int hours_to_work;
                printf("Сколько часов вы хотите потратить на работу? ");
                if (scanf("%d", &hours_to_work) != 1) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: введено не число!\n");
                    printf("----------------------------------------\n");
                    clear_input_buffer();
                    break;
                }

                if (hours_to_work < 0) {
                    printf("\n----------------------------------------\n");
                    printf("Количество часов не может быть отрицательным!\n");
                    printf("----------------------------------------\n");
                    break;
                }

                int total_hours = current_hour + hours_to_work;
                current_day += total_hours / 24;
                current_hour = total_hours % 24;

                printf("\n----------------------------------------\n");
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                printf("----------------------------------------\n");
                break;
            }

            case 3:
                printf("\n----------------------------------------\n");
                for (int i = 0; i < INVENTORY_SIZE; i++) {
                    if (inventory[i] == 0) {
                        printf("Слот %d: [0]\n", i);
                    } else {
                        printf("Слот %d: [%d] (%s)\n", i, inventory[i], get_item_name(inventory[i]));
                    }
                }
                printf("----------------------------------------\n");
                break;

            case 4: {
                int slot;
                int item_id;

                printf("Введите индекс слота (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot) != 1) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: введено не число!\n");
                    printf("----------------------------------------\n");
                    clear_input_buffer();
                    break;
                }

                if (slot < 0 || slot >= INVENTORY_SIZE) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: индекс выходит за границы массива (0..%d)!\n", INVENTORY_SIZE - 1);
                    printf("----------------------------------------\n");
                    break;
                }

                printf("Введите ID предмета (от 0 до 9): ");
                if (scanf("%d", &item_id) != 1) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: введено не число!\n");
                    printf("----------------------------------------\n");
                    clear_input_buffer();
                    break;
                }

                if (item_id < 0 || item_id > 9) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: ID предмета должен быть от 0 до 9!\n");
                    printf("----------------------------------------\n");
                    break;
                }

                inventory[slot] = item_id;
                printf("\n----------------------------------------\n");
                printf("В слот %d помещен предмет [%d] (%s).\n", slot, item_id, get_item_name(item_id));
                printf("----------------------------------------\n");
                break;
            }

            case 5: {
                int slot;
                printf("Введите индекс слота для очистки (от 0 до %d): ", INVENTORY_SIZE - 1);
                if (scanf("%d", &slot) != 1) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: введено не число!\n");
                    printf("----------------------------------------\n");
                    clear_input_buffer();
                    break;
                }

                if (slot < 0 || slot >= INVENTORY_SIZE) {
                    printf("\n----------------------------------------\n");
                    printf("Ошибка: индекс выходит за границы массива (0..%d)!\n", INVENTORY_SIZE - 1);
                    printf("----------------------------------------\n");
                    break;
                }

                inventory[slot] = 0;
                printf("\n----------------------------------------\n");
                printf("Слот %d очищен.\n", slot);
                printf("----------------------------------------\n");
                break;
            }

            default:
                printf("\n----------------------------------------\n");
                printf("Неверный пункт меню!\n");
                printf("----------------------------------------\n");
                break;
        }
    }

    return 0;
}
