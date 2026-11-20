#include "utils.h"

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void print_time(int day, int hour) {
    printf("\n----------------------------------------\n");
    printf("Текущее время: День %d, %02d:00\n", day, hour);
    if (hour >= 23 || hour < 2) {
        printf("Поздний вечер! До 02:00 ночи осталось не так много времени.\n");
    } else if (hour == 2) {
        printf("Внимание: на часах 02:00 ночи! Пора идти спать.\n");
    }
    printf("----------------------------------------\n");
}

int get_hours_until_curfew(int hour) {
    if (hour >= 8) {
        return (24 - hour) + 2;
    } else if (hour < 2) {
        return 2 - hour;
    } else {
        return 0;
    }
}

int can_wait_hours(int current_hour, int hours_to_wait) {
    int max_wait = get_hours_until_curfew(current_hour);
    return (hours_to_wait > 0 && hours_to_wait <= max_wait);
}

void wait_hours(int *day, int *hour, int hours_to_wait) {
    int total = *hour + hours_to_wait;
    *day += total / 24;
    *hour = total % 24;
}

void sleep_to_morning(int *day, int *hour) {
    if (*hour >= 8) {
        *day += 1;
    }
    *hour = 8;
}

void handle_time_menu(int *day, int *hour) {
    int running = 1;
    while (running) {
        printf("\n========================================\n");
        printf("    РЕЖИМ ДНЯ И УПРАВЛЕНИЕ ВРЕМЕНЕМ\n");
        printf("========================================\n");
        printf("Текущее время: День %d, %02d:00\n", *day, *hour);
        printf("[1] Посмотреть на часы\n");
        printf("[2] Пропустить время (Ждать)\n");
        printf("[3] Отдых (Лечь спать до 08:00 утра)\n");
        printf("[0] Вернуться в главное меню\n");
        printf("----------------------------------------\n");
        printf("Выберите действие: ");

        int choice;
        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("\nОшибка: введите число!\n");
            continue;
        }

        switch (choice) {
            case 0:
                running = 0;
                break;
            case 1:
                print_time(*day, *hour);
                break;
            case 2: {
                int max_wait = get_hours_until_curfew(*hour);
                if (max_wait <= 0) {
                    printf("\nНа часах уже %02d:00 ночи! Гулять дальше нельзя, пора лечь спать.\n", *hour);
                    break;
                }
                printf("Сколько часов пропустить (от 1 до %d)? ", max_wait);
                int h;
                if (scanf("%d", &h) != 1) {
                    clear_input_buffer();
                    printf("\nОшибка ввода числа!\n");
                    break;
                }
                if (h < 1) {
                    printf("\nЧисло часов должно быть не меньше 1!\n");
                    break;
                }
                if (h > max_wait) {
                    printf("\nНельзя мотать время дальше 02:00 ночи! Максимум можно подождать: %d ч.\n", max_wait);
                    break;
                }
                wait_hours(day, hour, h);
                printf("\nПрошло %d ч. ", h);
                print_time(*day, *hour);
                break;
            }
            case 3:
                sleep_to_morning(day, hour);
                printf("\nВы сладко проспали до утра! Наступил День %d, 08:00.\n", *day);
                running = 0;
                break;
            default:
                printf("\nНеверный пункт меню!\n");
                break;
        }
    }
}

int load_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    const char *defaults[MAX_ITEMS] = {
        "Пусто", "Дерево", "Камень", "Семена", "Морковь",
        "Пшеница", "Яблоко", "Яйцо", "Шерсть", "Молоко"
    };

    for (int i = 0; i < MAX_ITEMS; i++) {
        strncpy(item_names[i], defaults[i], MAX_NAME_LEN - 1);
        item_names[i][MAX_NAME_LEN - 1] = '\0';
    }

    FILE *f = fopen("items.txt", "r");
    if (f == NULL) {
        f = fopen("src/items.txt", "r");
    }

    if (f == NULL) {
        return 0;
    }

    int id;
    char name[MAX_NAME_LEN];
    while (fscanf(f, "%d %31s", &id, name) == 2) {
        if (id >= 0 && id < MAX_ITEMS) {
            strncpy(item_names[id], name, MAX_NAME_LEN - 1);
            item_names[id][MAX_NAME_LEN - 1] = '\0';
        }
    }

    fclose(f);
    return 1;
}

const char* get_item_name(int id, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    if (id >= 0 && id < MAX_ITEMS) {
        return item_names[id];
    }
    return "Неизвестный предмет";
}

int find_item_by_name(const char item_names[MAX_ITEMS][MAX_NAME_LEN], int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (strcmp(item_names[i], name) == 0) {
            return i;
        }
    }
    return -1;
}

int save_diary_entry(const char *filename, const char *farmer_name, int day, int hour, const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    FILE *f = fopen(filename, "a");
    if (f == NULL) {
        return 0;
    }

    fprintf(f, "=== Запись в дневнике ===\n");
    fprintf(f, "Фермер: %s\n", farmer_name);
    fprintf(f, "Время: День %d, %02d:00\n", day, hour);
    fprintf(f, "Инвентарь:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            fprintf(f, "  Слот %d: Пусто\n", i);
        } else {
            fprintf(f, "  Слот %d: %s (ID: %d)\n", i, get_item_name(inventory[i], item_names), inventory[i]);
        }
    }
    fprintf(f, "=========================\n\n");

    fclose(f);
    return 1;
}

void handle_save_diary(const char *farmer_name, int day, int hour, const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]) {
    printf("\n----------------------------------------\n");
    printf("=== Сводка состояния ===\n");
    printf("Фермер: %s\n", farmer_name);
    printf("Время: День %d, %02d:00\n", day, hour);
    printf("Инвентарь:\n");
    for (int i = 0; i < size; i++) {
        if (inventory[i] == 0) {
            printf("  Слот %d: Пусто\n", i);
        } else {
            printf("  Слот %d: %s (ID: %d)\n", i, get_item_name(inventory[i], item_names), inventory[i]);
        }
    }
    printf("----------------------------------------\n");

    if (save_diary_entry("diary.txt", farmer_name, day, hour, inventory, size, item_names)) {
        printf("Запись успешно сохранена в diary.txt!\n");
    } else {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n");
    }
    printf("----------------------------------------\n");
}

int decompress_rle_line(const char *compressed, char *decompressed, int max_len) {
    int in_idx = 0;
    int out_idx = 0;

    while (compressed[in_idx] != '\0') {
        int count = 0;
        int has_digits = 0;

        while (compressed[in_idx] >= '0' && compressed[in_idx] <= '9') {
            has_digits = 1;
            count = count * 10 + (compressed[in_idx] - '0');
            in_idx++;
        }

        if (!has_digits) {
            count = 1;
        }

        if (compressed[in_idx] == '\0') {
            return -1;
        }

        char ch = compressed[in_idx++];

        for (int i = 0; i < count; i++) {
            if (out_idx >= max_len - 1) {
                return -2;
            }
            decompressed[out_idx++] = ch;
        }
    }

    decompressed[out_idx] = '\0';
    return out_idx;
}

int unpack_map(const char *in_filename, const char *out_filename, char map[MAX_MAP_ROWS][MAX_MAP_COLS], int *rows, int *cols, char *error_msg, int max_err_len) {
    FILE *in = fopen(in_filename, "r");
    if (in == NULL) {
        char fallback[64];
        snprintf(fallback, sizeof(fallback), "src/%s", in_filename);
        in = fopen(fallback, "r");
    }

    if (in == NULL) {
        snprintf(error_msg, max_err_len, "Ошибка: не удалось открыть файл \"%s\"", in_filename);
        return 0;
    }

    char buffer[MAX_LINE_LEN];
    *rows = 0;
    *cols = 0;
    int expected_len = -1;
    int success = 1;
    while (fgets(buffer, sizeof(buffer), in) != NULL) {
        size_t len = strlen(buffer);
        while (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n')) {
            buffer[--len] = '\0';
        }

        if (len == 0) {
            continue;
        }

        if (*rows >= MAX_MAP_ROWS) {
            snprintf(error_msg, max_err_len, "Ошибка: превышено максимальное кол-во строк карты (%d)!", MAX_MAP_ROWS);
            success = 0;
            break;
        }

        int decomp_len = decompress_rle_line(buffer, map[*rows], MAX_MAP_COLS);
        if (decomp_len == -1) {
            snprintf(error_msg, max_err_len, "Ошибка в строке %d: некорректный формат RLE (\"%s\")", *rows + 1, buffer);
            success = 0;
            break;
        } else if (decomp_len == -2) {
            snprintf(error_msg, max_err_len, "Ошибка в строке %d: превышена максимальная длина строки (%d символов)", *rows + 1, MAX_MAP_COLS - 1);
            success = 0;
            break;
        }

        if (*rows == 0) {
            expected_len = decomp_len;
            *cols = decomp_len;
        } else if (decomp_len != expected_len) {
            snprintf(error_msg, max_err_len, "Ошибка геометрии карты в строке %d: получена длина %d (ожидалась длина %d)", *rows + 1, decomp_len, expected_len);
            success = 0;
            break;
        }

        (*rows)++;
    }

    fclose(in);

    if (success && *rows == 0) {
        snprintf(error_msg, max_err_len, "Ошибка: файл \"%s\" пуст или не содержит данных карты!", in_filename);
        success = 0;
    }
    FILE *out = fopen(out_filename, "w");
    if (out == NULL) {
        char fallback[64];
        snprintf(fallback, sizeof(fallback), "src/%s", out_filename);
        out = fopen(fallback, "w");
    }

    if (out != NULL) {
        if (!success) {
            fprintf(out, "%s\n", error_msg);
        } else {
            for (int i = 0; i < *rows; i++) {
                fprintf(out, "%s\n", map[i]);
            }
        }
        fclose(out);
    }

    return success;
}

void handle_unpack_map(void) {
    char map[MAX_MAP_ROWS][MAX_MAP_COLS];
    int rows = 0;
    int cols = 0;
    char error_msg[256] = "";

    printf("\n----------------------------------------\n");

    int ok = unpack_map("input.txt", "output.txt", map, &rows, &cols, error_msg, sizeof(error_msg));

    if (ok) {
        printf("\nКарта успешно распакована! (Размер: %d x %d)\n\n", cols, rows);
        for (int i = 0; i < rows; i++) {
            printf("%s\n", map[i]);
        }
        printf("\nРаспакованная карта сохранена в output.txt.\n");
    } else {
        printf("\n%s\n", error_msg);
        printf("Сообщение об ошибке записано в output.txt.\n");
    }
    printf("----------------------------------------\n");
}
