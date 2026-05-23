#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "raygui/src/raygui.h"
#include "raygui/styles/bluish/style_bluish.h"

#include <limits.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define CHECKBOX_SIZE 15

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raygui - controls test suite");
    SetTargetFPS(60);

    int width = GetScreenWidth();
    int height = GetScreenHeight();

    Vector2 center = {width/2, height/2};

    bool showMessageBox = false;

    GuiLoadStyleBluish();

    //gui variables
    bool binary_split = true;
    bool chunking_enabled = true;
    long num_digits = 10;

    char *tabs[3] = { "General", "Controls", "Advanced" };
    int activeTab = 0;

    while (!WindowShouldClose())
    {

        //get screen width and height for proper scaling
        width = GetScreenWidth();
        height = GetScreenHeight();

        //settings positions
        Vector2 settings_box_pos = {SCREEN_WIDTH-120-10, 40};
        Rectangle settings_box_rect = (Rectangle){ settings_box_pos.x, settings_box_pos.y, 120, 300 };

        Rectangle binary_split_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y+10, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle chunking_checkbox = (Rectangle){ settings_box_pos.x+10, settings_box_pos.y + 15 + CHECKBOX_SIZE, CHECKBOX_SIZE, CHECKBOX_SIZE };

        Rectangle num_digits_text = (Rectangle){center.x-(125/2), 80-30, 125, 30};
        Rectangle num_digits_inp = (Rectangle){center.x-(125/2), 80, 125, 30};

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            GuiGroupBox(settings_box_rect, "Settings");
            GuiCheckBox(binary_split_checkbox, "Binary Split", &binary_split);
            GuiCheckBox(chunking_checkbox, "Enable Chunking", &chunking_enabled);

            GuiSetStyle(LABEL, TEXT_ALIGNMENT, TEXT_ALIGN_CENTER);
            GuiLabel(num_digits_text, "Digits to Calculate");
            GuiValueBox(num_digits_inp, NULL, &num_digits, 1, LONG_MAX, true);

            if (GuiTabBar((Rectangle){ 0, 0, 300, 30 }, tabs, 3, 0, &activeTab) != -1) {
                // A tab was selected to be closed
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}