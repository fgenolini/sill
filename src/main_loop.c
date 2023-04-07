#include "raylib.h"

#include "init.h"
#include "load_resources.h"

bool has_sill_texture = false;
int sill_texture_x = 0;
int sill_texture_y = 0;

static float delta_time = 0.0f;
static Vector2 dice_position = {0};
static Vector2 dice_speed = {200.0f, 140.0f};
static float dice_radius = 50.0;
static Vector2 dice_size = {0};

static void initial_variables()
{
    dice_position.x = screen_width / 2.0f;
    dice_position.y = screen_height / 2.0f;
    dice_size.x = dice_radius * 2.0f;
    dice_size.y = dice_radius * 2.0f;
}

static bool out_of_bounds_x(float x, float width)
{
    return x + width >= screen_width || x <= 0.0f;
}

static bool out_of_bounds_y(float y, float height)
{
    return y + height >= screen_height || y <= 0.0f;
}

static void update_variables()
{
    delta_time = GetFrameTime();
    dice_position.x += dice_speed.x * delta_time;
    dice_position.y += dice_speed.y * delta_time;
    if (out_of_bounds_x(dice_position.x, dice_size.x))
        dice_speed.x *= -1.0f;

    if (out_of_bounds_y(dice_position.y, dice_size.y))
        dice_speed.y *= -1.0f;
}

static void draw_contents()
{
    if (has_sill_texture)
        DrawTexture(sill_texture, sill_texture_x, sill_texture_y, WHITE);

    DrawText("press escape to exit",
             190, 200, 20, LIGHTGRAY);
    DrawRectangleV(dice_position, dice_size, DARKGREEN);
}

static void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_contents();
    DrawFPS(10, 10);
    EndDrawing();
}

void main_loop()
{
    initial_variables();
    while (!WindowShouldClose())
    {
        update_variables();
        draw();
    }
}
