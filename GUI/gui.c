#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#include "raygui/styles/jungle/style_jungle.h"

#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>



#include "../calc_pi.h"
#include "../helpers.h"
#include "../mandelbrot.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400
#define CHECKBOX_SIZE 15

//PI stuff
#define NUM_DIGITS 13000000
#define ACCURACY 10000000
#define DIGITS_PER_LINE 50

//printing stuff
#define MAX_PRINTF_LENGTH 4095

#include "gui.h"

static char *display_value;
static unsigned int display_value_max_len;
static unsigned int display_value_len;

static float progressbar_value = 0;

void start_gui()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raygui - controls test suite");
    SetTargetFPS(60);

    display_value = calloc(1024, 1);
    display_value_max_len = 1024;
    display_value_len = 0;

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    bool showMessageBox = false;

    GuiLoadStyleJungle();

    //gui variables
    bool binary_split = true;
    bool chunking_enabled = true;
    long num_digits = 10;
    bool clear = true;

    char *tabs[3] = { "Calculate", "Mandelbrot", "Noise" };
    int activeTab = 0;

    while (!WindowShouldClose())
    {

        //get screen width and height for proper scaling
        width = GetScreenWidth();
        height = GetScreenHeight();
        Vector2 center = {width/2, height/2};

        //settings positions
        Vector2 settings_box_pos = {width-140-10, 40};
        Rectangle settings_box_rect = (Rectangle){ settings_box_pos.x, settings_box_pos.y, 140, 300 };

        Rectangle binary_split_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y+10, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle chunking_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y + 15 + CHECKBOX_SIZE, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle num_digits_text = (Rectangle){center.x-(225/2), 80-30, 225, 30};
        Rectangle num_digits_inp = (Rectangle){center.x-(125/2), 80, 125, 30};

        Rectangle calc_button = (Rectangle){num_digits_inp.x, num_digits_inp.y+num_digits_inp.height+5, 125, 30};

        Rectangle progress_bar = (Rectangle){center.x-(300/2), calc_button.y+calc_button.height+15, 300, 10};

        Vector2 output_box_pos = {15, progress_bar.y+progress_bar.height+15};
        Rectangle output_box = (Rectangle){output_box_pos.x, output_box_pos.y, width-175, height-output_box_pos.y-15};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            GuiTabBar((Rectangle){ 0, 0, width, 30 }, tabs, 3, 0, &activeTab);
            if (activeTab==0) {
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                GuiGroupBox(settings_box_rect, "Settings");
                GuiCheckBox(binary_split_checkbox, "Binary Split", &binary_split);
                GuiCheckBox(chunking_checkbox, "Enable Chunking", &chunking_enabled);

                GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
                GuiLabel(num_digits_text, "Digits to Calculate");
                GuiValueBox(num_digits_inp, NULL, &num_digits, 1, LONG_MAX, true);
                if (GuiButton(calc_button, "Calculate")==true) {
                    pthread_t calc_thread;
                    run_calculation_args args = {num_digits, display_value, &display_value_max_len, &display_value_len};
                    pthread_create(&calc_thread, NULL, start_calculation, &args);
                }

                GuiProgressBar(progress_bar, "Percent Calculated:\t", "100%", &progressbar_value, 0, 100);

                GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_TOP);   // WARNING: Word-wrap does not work as expected in case of no-top alignment
                GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_WORD);            // WARNING: If wrap mode enabled, text editing is not supported
                GuiTextBox(output_box, display_value, 1024, false);
                GuiSetStyle(DEFAULT, TEXT_WRAP_MODE, TEXT_WRAP_NONE);
                GuiSetStyle(DEFAULT, TEXT_ALIGNMENT_VERTICAL, TEXT_ALIGN_MIDDLE);
            }else if (activeTab==1) {
                //only clear screen if we're not currently drawing so we don't have to keep recalculating/drawing the set
                if (clear) {
                    ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
                }
                if (GuiButton((Rectangle){center.x-(125/2), 30, 125, 30}, "Draw")==true) {
                    clear = false;
                    pthread_t draw_mandelbrot_thread;
                    Image mandelbrot_image = GenImageColor(width, height, WHITE);
                    draw_mandelbrot_args args = {width, height, 100, &mandelbrot_image};
                    pthread_create(&draw_mandelbrot_thread, NULL, draw_mandelbrot_img, &args);
                    pthread_join(draw_mandelbrot_thread, NULL);
                    Texture2D mandelbrot_texture = LoadTextureFromImage(mandelbrot_image);
                    DrawTexture(mandelbrot_texture, 0, 0, WHITE);
                }
            }else {
                ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
            }

        EndDrawing();
    }

    CloseWindow();
}

static void* start_calculation(void* func_args) {
    run_calculation_args args = *((run_calculation_args*) func_args);
    uint64_t num_digits = args.num_digits;

    str_append(args.display_str, "Calculating PI...\n", args.display_str_max_len, args.display_str_len, sizeof("Calculating PI...\n"));
    clock_t begin = clock();
    calc_pi_chunking_binary_split(num_digits, 20, 1); //for some reason parameters are swapped
    //char *pi_str = calc_pi(NUM_DIGITS, ACCURACY);
    clock_t end = clock();

    double calculation_time = (double) (end-begin)/CLOCKS_PER_SEC;

    char digits_calc_text[200] = {0}; //200 digits should be more than enough
    snprintf(digits_calc_text, sizeof(digits_calc_text), "Digits Calculated: %ld. Time taken: %f.\n", num_digits, calculation_time);

    str_append(args.display_str, digits_calc_text, args.display_str_max_len, args.display_str_len, sizeof(digits_calc_text));
    str_append(args.display_str, "Output written to pi.txt\n", args.display_str_max_len, args.display_str_len, sizeof("Output written to pi.txt\n"));
    pthread_exit(0);
}

void print_to_calc_box(char* str, int str_len) {
    if (strchr(str, '\r')!=NULL) {
        //str_inv_set_line(display_value, str, 1, &display_value_max_len, &display_value_len, str_len);
    }else {
        str_append(display_value, str, &display_value_max_len, &display_value_len, str_len);
    }
}

void update_calc_progress_bar(float value) {
    progressbar_value = value;
}

static void* draw_mandelbrot_img(void* func_args) {
    draw_mandelbrot_args args = *((draw_mandelbrot_args*) func_args);
    unsigned int width = args.width;
    unsigned int height = args.height;
    unsigned int max_iterations = args.max_iterations;
    Image *mandelbrot_image = args.image;

    int min_re = -2;
    int max_re = 1;
    int min_im = -1;
    int max_im = 1;

    double re_step = (max_re - min_re) / (double) width;
    double im_step = (max_im - min_im) / (double) height;

    Color *mandelbrot_pixels = (Color *) mandelbrot_image->data;

    for (int x=0;x<width;x++) {
        ldouble re = re_step * x + min_re;
        for (int y=0;y<height;y++) {
            ldouble im = im_step * y + min_im;
            complex_number num = {re, im};
            unsigned int num_iter = get_num_iterations(num, max_iterations);
            if (num_iter == max_iterations) {
                mandelbrot_pixels[width * y + x] = BLACK;
            }else {
                unsigned int hue = 30 + (120 * num_iter / (double) max_iterations);
                float hsl[3] = {hue/60.0, 1.0, 0.5};
                float rgb[3] = {0};
                HSLtoRGB(hsl, rgb);
                mandelbrot_pixels[width * y + x] = (Color) {rgb[0]*255, rgb[1]*255, rgb[2]*255, 255};
            }
        }
    }

    // ldouble re = min_re;
    // while (re < max_re) {
    //     ldouble im = min_im;
    //     while (im < max_im) {
    //         int x = (re - min_re) / re_step;
    //         int y = (im - min_im) / im_step;
    //         //printf("re: %Lf | im: %Lf\n",re, im);
    //         complex_number num = {re, im};
    //         if (in_set(num, max_iterations)) {
    //             mandelbrot_pixels[width * y + x] = BLACK;
    //         }
    //         im+=im_step;
    //     }
    //     re+=re_step;
    // }

    printf("done calc\n");

    pthread_exit(0);
}