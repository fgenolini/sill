#include "raylib.h"

#include "load_resources.h"
#include "main_loop.h"
#include "object/collision.h"
#include "object/object.h"
#include "object/text.h"

static void draw_contents()
{
    if (main_state.has_sill_texture)
        DrawTexture(main_state.sill_texture,
                    main_state.sill_texture_x, main_state.sill_texture_y,
                    WHITE);
    draw_objects();
    draw_text();
}

static void draw()
{
    BeginDrawing();
    {
        ClearBackground(RAYWHITE);
        draw_contents();
    }
    EndDrawing();
}

void main_loop()
{
    initialise_objects();
    initialise_collision_pairs();
    while (!WindowShouldClose())
    {
        main_state.delta_time = GetFrameTime();
        update_objects();
        draw();
    }
}
