#include <stdio.h>

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

int main(void) {
    int current_day = 1;
    int current_hour = 8;
    int is_running = 1;
    int choice;

    while (is_running) {
        printf("\n=== МЕНЮ ФЕРМЫ ===\n");
        printf("[0] Выход\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Промотать время (Поработать)\n");
        printf("Выберите пункт меню: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка: необходимо ввести целое число!\n");
            clear_input_buffer();
            continue;
        }

        switch (choice) {
            case 0:
                printf("Завершение работы программы.\n");
                is_running = 0;
                break;

            case 1:
                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                break;

            case 2: {
                int hours_to_work;
                printf("Сколько часов вы хотите потратить на работу? ");
                if (scanf("%d", &hours_to_work) != 1) {
                    printf("Ошибка: введено не число!\n");
                    clear_input_buffer();
                    break;
                }

                if (hours_to_work < 0) {
                    printf("Количество часов не может быть отрицательным!\n");
                    break;
                }

                int total_hours = current_hour + hours_to_work;
                current_day += total_hours / 24;
                current_hour = total_hours % 24;

                printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
                break;
            }

            default:
                printf("Неверный пункт меню!\n");
                break;
        }
    }

    return 0;
}
