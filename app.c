#include <stdio.h>
#include <ctype.h>
#include <string.h>
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
    static int frame_counter = 0;
    frame_counter++;

    if (frame_counter % 5 == 0) {
        if (IsKeyDown(KEY_BACKSPACE)) {
            (*letter_count)--;
            if (*letter_count < 0) 
                *letter_count = 0;
            input_text[*letter_count] = '\0';
        }
    }
}

void get_key(char input_text[], int *letter_count) 
{
    int key = GetCharPressed();
    while (key > 0) {
        int is_operator = (key == '+' || key == '-' || key == '*' || 
                           key == '/' || key == '.' || key == '(' || key == ')');
        if (isdigit(key) || is_operator || key == '.') {
            store_number(input_text, key, letter_count);
        }
        key = GetCharPressed();
    }

}

void eval_expr(char input_text[], int *letter_count) 
{
    if (IsKeyPressed(KEY_ENTER)) {
        double num1 = 0, num2 = 0, result = 0;
        char op = '\0';
        int items_read = sscanf(input_text, "%lf %c %lf", &num1, &op, &num2);
        if (items_read == 3) {
            switch (op) {
                case '+': result = num1 + num2; break;
                case '-': result = num1 - num2; break;
                case '*': result = num1 * num2; break;
                case '/': 
                    if (num2 != 0) result = num1 / num2;
                    else result = 0;  // Handle divide by zero
                    break;
                default: result = 0;
            }
            
            // Convert result back to string
            snprintf(input_text, MAX_INPUT_CHARS, "%.2f", result);
            *letter_count = strlen(input_text);
        } else {
            // If can't parse, show error
            const char *error = "Error";
            strncpy(input_text, error, MAX_INPUT_CHARS);
            *letter_count = strlen(error);
        }
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
        eval_expr(input_text, &letter_count);

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

