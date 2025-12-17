#include <stdio.h>
#include "raylib.h"

// constants
const int SCREEN_WIDTH = 450;
const int SCREEN_HEIGHT = 550;
const char *WINDOW_TITLE = "Calculator";
const int MAX_INPUT_CHARS = 30;
const int FPS = 60;



void store_number(char input_text[], int key, int *letter_count)
{
    if (*letter_count < MAX_INPUT_CHARS) {
        // store char and update end of char
        input_text[*letter_count] = (char)key;
        input_text[*letter_count + 1] = '\0';
        (*letter_count)++;
    }
}

void backspace_pressed(char input_text[], int *letter_count) 
{
    if (IsKeyDown(KEY_BACKSPACE)) {
        (*letter_count)--;
        if (*letter_count < 0) 
            *letter_count = 0;
        input_text[*letter_count] = '\0';
    }
}

void get_key(char input_text[], int *letter_count) 
{
    int key = GetCharPressed();

    while (key > 0) {
        store_number(input_text, key, &letter_count);
        key = GetCharPressed();
    }

}

int main(void) 
{

    // array to store typed letters
    char input_text[MAX_INPUT_CHARS + 1];
    input_text[0] = '\0';
    int letter_count = 0;
    // bool mouseOnText = false;

    // x, y, w, h
    // for width: 10 for x 10 for space form right
    Rectangle text_box = {10, 10, SCREEN_WIDTH - 20, 50 };

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        get_key(input_text, &letter_count);
        backspace_pressed(input_text, &letter_count);

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangleRec(text_box, LIGHTGRAY);
        DrawRectangleLinesEx(text_box, 2, DARKGRAY);

        DrawText(input_text, (int)text_box.x + 5, (int)text_box.y + 10, 30, MAROON);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

