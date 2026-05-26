#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#include "raygui/styles/bluish/style_bluish.h"

#include <limits.h>
#include <time.h>
#include <pthread.h>

#include "../calc_pi.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 400
#define CHECKBOX_SIZE 15

//PI stuff
#define NUM_DIGITS 13000000
#define ACCURACY 10000000
#define DIGITS_PER_LINE 50

void* start_calculation(void* arg);

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raygui - controls test suite");
    SetTargetFPS(60);

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

    char *display_value = calloc(1000, 1); //max size of 1000

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
                    pthread_create(&calc_thread, NULL, start_calculation, NULL);
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
    return 0;
}

void* start_calculation(void* arg) {
    printf("Calculating PI...\n");

    clock_t begin = clock();
    calc_pi_chunking_binary_split(NUM_DIGITS, 20, 1); //for some reason parameters are swapped
    //char *pi_str = calc_pi(NUM_DIGITS, ACCURACY);
    clock_t end = clock();

    double calculation_time = (double) (end-begin)/CLOCKS_PER_SEC;

    printf("done calculating!\n");

    printf("Digits Calculated: %d. Time taken: %f.\n", NUM_DIGITS, calculation_time);
}