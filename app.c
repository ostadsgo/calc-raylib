
#include <stdio.h>
#include "raylib.h"

int main() {
    // constants
    const int SCREEN_WIDTH = 450;
    const int SCREEN_HEIGHT = 550;
    const char *WINDOW_TITLE = "Calculator";
    const int MAX_INPUT_CHARS = 30;
    const int FPS = 60;

    // array to store typed letters
    char inputText[MAX_INPUT_CHARS + 1];
    inputText[0] = '\0';
    int letterCount = 0;
    // bool mouseOnText = false;

    // x, y, w, h
    // for width: 10 for x 10 for space form right
    Rectangle textBox = {10, 10, SCREEN_WIDTH - 20, 50 };


    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FPS);

    while (!WindowShouldClose()) {
        int key = GetCharPressed();

        while (key > 0) {
            printf("The key pressed: %c", key);
            if (letterCount < MAX_INPUT_CHARS) {
                // store char and update end of char
                inputText[letterCount] = (char)key;
                inputText[letterCount + 1] = '\0';
                letterCount++;
            }
            // get next char
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            letterCount--;
        }

        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawRectangleRec(textBox, LIGHTGRAY);
        DrawRectangleLinesEx(textBox, 2, DARKGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
