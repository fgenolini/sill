#include "raylib.h"

void draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    DrawText("press escape to exit",
             190, 200, 20, LIGHTGRAY);

    EndDrawing();
}

void main_loop()
{
    while (!WindowShouldClose())
    {
        draw();
    }
}
