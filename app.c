#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "raylib.h"

// constants
const int SCREEN_WIDTH = 330;
const int SCREEN_HEIGHT = 430;
const char *WINDOW_TITLE = "Calculator";
const int MAX_INPUT_CHARS = 20;
const int FPS = 60;
const int BUTTON_COUNT = 16;  // 0-9, +, -, *, /, ., =, C
const int BUTTON_WIDTH = 70;
const int BUTTON_HEIGHT = 70;
const int BUTTON_MARGIN = 10;
const int BUTTON_Y_START = 100;  // Below text box

typedef struct {
    Rectangle bounds; // x, y, w, h
    char label[5];
    Color color;
} Button;

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

void eval_expr(char input_text[], int *letter_count, int trigger_from_enter) 
{
    if (trigger_from_enter == 1 ? IsKeyPressed(KEY_ENTER) : 1) {
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

void init_buttons(Button buttons[]) 
{
    // FIXED: Correct array initialization
    const char *labels[] = {"7", "8", "9", "/",
                           "4", "5", "6", "*",
                           "1", "2", "3", "-",
                           "C", "0", ".", "="};
    
    for (int i = 0; i < BUTTON_COUNT; i++) {
        int row = i / 4;  // 4 buttons per row
        int col = i % 4;
        
        buttons[i].bounds.x = 10 + col * (BUTTON_WIDTH + BUTTON_MARGIN);
        buttons[i].bounds.y = BUTTON_Y_START + row * (BUTTON_HEIGHT + BUTTON_MARGIN);
        buttons[i].bounds.width = BUTTON_WIDTH;
        buttons[i].bounds.height = BUTTON_HEIGHT;
        
        strcpy(buttons[i].label, labels[i]);
        buttons[i].color = LIGHTGRAY;
    }
}

void draw_buttons(Button buttons[])
{
    for (int i = 0; i < BUTTON_COUNT; i++) {
        // Draw button rectangle
        DrawRectangleRec(buttons[i].bounds, buttons[i].color);
        DrawRectangleLinesEx(buttons[i].bounds, 2, DARKGRAY);
        
        // Center text in button
        int text_width = MeasureText(buttons[i].label, 30);
        int text_x = buttons[i].bounds.x + (BUTTON_WIDTH - text_width) / 2;
        int text_y = buttons[i].bounds.y + (BUTTON_HEIGHT - 30) / 2;
        
        DrawText(buttons[i].label, text_x, text_y, 30, DARKBLUE);
    }
}

void handle_button_clicks(Button buttons[], char input_text[], int *letter_count)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse_pos = GetMousePosition();
        
        for (int i = 0; i < BUTTON_COUNT; i++) {
            if (CheckCollisionPointRec(mouse_pos, buttons[i].bounds)) {
                // Handle different buttons
                if (strcmp(buttons[i].label, "=") == 0) {
                    // Evaluate expression
                    eval_expr(input_text, letter_count, 0);
                } else if (strcmp(buttons[i].label, "C") == 0) {
                    // Clear button
                    input_text[0] = '\0';
                    *letter_count = 0;
                } else {
                    // Number or operator button
                    if (*letter_count < MAX_INPUT_CHARS) {
                        input_text[*letter_count] = buttons[i].label[0];
                        input_text[*letter_count + 1] = '\0';
                        (*letter_count)++;
                    }
                }
                break;  // Only handle one button per click
            }
        }
    }
}

int main(void) 
{
    // array to store typed letters
    char input_text[MAX_INPUT_CHARS + 1];
    input_text[0] = '\0';
    int letter_count = 0;

    // x, y, w, h
    // for width: 10 for x 10 for space form right
    Rectangle text_box = {10, 10, SCREEN_WIDTH - 20, 50 };

    // buttons
    Button buttons[BUTTON_COUNT];
    init_buttons(buttons);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        get_key(input_text, &letter_count);
        backspace_pressed(input_text, &letter_count);
        eval_expr(input_text, &letter_count, 1);
        handle_button_clicks(buttons, input_text, &letter_count);
        
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangleRec(text_box, LIGHTGRAY);
        DrawRectangleLinesEx(text_box, 2, DARKGRAY);

        DrawText(input_text, (int)text_box.x + 5, (int)text_box.y + 10, 30, MAROON);

        draw_buttons(buttons);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
