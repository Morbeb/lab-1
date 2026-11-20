#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <string.h>

#define INVENTORY_SIZE 10
#define MAX_ITEMS 10
#define MAX_NAME_LEN 32
#define MAX_MAP_ROWS 100
#define MAX_MAP_COLS 256
#define MAX_LINE_LEN 256

// Вспомогательные функции ввода
void clear_input_buffer(void);

// Время и режим дня
void print_time(int day, int hour);
int get_hours_until_curfew(int hour);
int can_wait_hours(int current_hour, int hours_to_wait);
void wait_hours(int *day, int *hour, int hours_to_wait);
void sleep_to_morning(int *day, int *hour);
void handle_time_menu(int *day, int *hour);

// Работа с каталогом предметов
int load_items(char item_names[MAX_ITEMS][MAX_NAME_LEN]);
const char* get_item_name(int id, const char item_names[MAX_ITEMS][MAX_NAME_LEN]);
int find_item_by_name(const char item_names[MAX_ITEMS][MAX_NAME_LEN], int count, const char *name);

// Дневник фермера
int save_diary_entry(const char *filename, const char *farmer_name, int day, int hour, const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]);
void handle_save_diary(const char *farmer_name, int day, int hour, const int inventory[], int size, const char item_names[MAX_ITEMS][MAX_NAME_LEN]);

// Распаковщик RLE (Лабораторная работа №3)
int decompress_rle_line(const char *compressed, char *decompressed, int max_len);
int unpack_map(const char *in_filename, const char *out_filename, char map[MAX_MAP_ROWS][MAX_MAP_COLS], int *rows, int *cols, char *error_msg, int max_err_len);
void handle_unpack_map(void);

#endif // UTILS_H
