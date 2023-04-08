#include "raylib.h"

void draw_text()
{
    DrawFPS(10, 10);
    DrawText("Press escape to exit", 190, 200, 20, LIGHTGRAY);
}
