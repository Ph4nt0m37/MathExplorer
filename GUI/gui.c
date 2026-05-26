#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#include "raygui/styles/bluish/style_bluish.h"

#include <limits.h>
#include <time.h>
#include <pthread.h>
#include <stdint.h>
#include <stdarg.h>



#include "../calc_pi.h"
#include "../helpers.h"

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

char *display_value;
unsigned int display_value_max_len;
unsigned int display_value_len;

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

    GuiLoadStyleBluish();

    //gui variables
    bool binary_split = true;
    bool chunking_enabled = true;
    long num_digits = 10;

    char *tabs[3] = { "General", "Controls", "Advanced" };
    int activeTab = 0;

    float progressbar_value = 0;

    while (!WindowShouldClose())
    {

        //get screen width and height for proper scaling
        width = GetScreenWidth();
        height = GetScreenHeight();

        Vector2 center = {width/2, height/2};

        //settings positions
        Vector2 settings_box_pos = {width-120-10, 40};
        Rectangle settings_box_rect = (Rectangle){ settings_box_pos.x, settings_box_pos.y, 120, 300 };

        Rectangle binary_split_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y+10, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle chunking_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y + 15 + CHECKBOX_SIZE, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle num_digits_text = (Rectangle){center.x-(125/2), 80-30, 125, 30};
        Rectangle num_digits_inp = (Rectangle){center.x-(125/2), 80, 125, 30};

        Rectangle calc_button = (Rectangle){num_digits_inp.x, num_digits_inp.y+num_digits_inp.height+5, 125, 30};

        Rectangle progress_bar = (Rectangle){center.x-(300/2), calc_button.y+calc_button.height+15, 300, 10};

        Vector2 output_box_pos = {15, progress_bar.y+progress_bar.height+15};
        Rectangle output_box = (Rectangle){output_box_pos.x, output_box_pos.y, width-175, height-output_box_pos.y-15};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            GuiTabBar((Rectangle){ 0, 0, width, 30 }, tabs, 3, 0, &activeTab);
            if (activeTab==0) {
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
}

void print_to_calc_box(char* str) {
    if (strchr(str, '\r')!=NULL) {
        str_inv_set_line(display_value, str, 1, &display_value_max_len, &display_value_len, sizeof(str));
    }else {
        str_append(display_value, str, &display_value_max_len, &display_value_len, sizeof(str));
    }
}