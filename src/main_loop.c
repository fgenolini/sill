#include "raylib.h"

#include "init.h"
#include "load_resources.h"

#define dice_radius 50.0f
#define dice_width dice_radius * 2.0f

int sill_texture_x = 0;
int sill_texture_y = 0;
bool has_sill_texture = false;

static const Vector2 dice_origin = {dice_radius, dice_radius};

static Vector2 dice_position = {0};
static Vector2 dice_speed = {200.0f, 140.0f};
static float delta_time = 0.0f;
static float dice_rotation = 0.0f;
static float height_minus_dice = 0.0f;
static float rotation_speed = 20.0f;
static float width_minus_dice = 0.0f;

static void initial_variables()
{
    dice_position.x = screen_width / 2.0f;
    dice_position.y = screen_height / 2.0f;
    width_minus_dice = screen_width - dice_radius;
    height_minus_dice = screen_height - dice_radius;
}

static bool out_of_bounds_x(float x)
{
    return x <= dice_radius || x >= width_minus_dice;
}

static bool out_of_bounds_y(float y)
{
    return y <= dice_radius || y >= height_minus_dice;
}

static void update_variables()
{
    delta_time = GetFrameTime();
    dice_position.x += dice_speed.x * delta_time;
    dice_position.y += dice_speed.y * delta_time;
    dice_rotation += rotation_speed * delta_time;
    if (out_of_bounds_x(dice_position.x))
    {
        dice_speed.x *= -1.0f;
        rotation_speed *= -1.0f;
    }

    if (out_of_bounds_y(dice_position.y))
    {
        dice_speed.y *= -1.0f;
        rotation_speed *= -1.0f;
    }
}

static void draw_contents()
{
    if (has_sill_texture)
        DrawTexture(sill_texture, sill_texture_x, sill_texture_y, WHITE);

    DrawText("press escape to exit",
             190, 200, 20, LIGHTGRAY);
    DrawRectanglePro(
        (Rectangle){dice_position.x, dice_position.y, dice_width, dice_width},
        dice_origin, dice_rotation, DARKGREEN);
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
