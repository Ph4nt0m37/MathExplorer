#ifndef GUI_HEADER
#define GUI_HEADER

#include <stdint.h>
#include "raylib.h"
#include "../complex_number.h"

typedef struct {
    uint64_t num_digits;
    char *display_str;
    unsigned int *display_str_max_len;
    unsigned int *display_str_len;
} run_calculation_args;

typedef struct {
    unsigned int width;
    unsigned int height;
    ldouble re_center;
    ldouble im_center;
    double zoom;
    unsigned int max_iterations;
    Image *image;
} draw_mandelbrot_args;

void start_gui();
static void* start_calculation(void* arg);
void print_to_calc_box(char* str, int str_size);
void update_calc_progress_bar(float value);
static void* draw_mandelbrot_img(void* func_args);

#endif