#ifndef GUI_HEADER
#define GUI_HEADER

#include <stdint.h>

typedef struct {
    uint64_t num_digits;
    char *display_str;
    unsigned int *display_str_max_len;
    unsigned int *display_str_len;
} run_calculation_args;

void start_gui();
static void* start_calculation(void* arg);
void print_to_calc_box(char* str);

#endif