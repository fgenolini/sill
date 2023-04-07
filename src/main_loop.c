#include "raylib.h"

#include "init.h"
#include "load_resources.h"

void draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    if (sill_texture.id)
    {
        DrawTexture(sill_texture, screen_width / 2 - sill_texture.width / 2,
                    screen_height / 2 - sill_texture.height / 2, WHITE);
    }

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
