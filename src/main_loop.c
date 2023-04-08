#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "init.h"
#include "load_resources.h"
#include "main_loop.h"

#define dice_radius 50.0f
#define dice_width dice_radius * 2.0f
#define egg_radius 20.0f
#define egg_width egg_radius * 2.0f
#define height_minus_dice screen_height - dice_radius
#define height_minus_egg screen_height - egg_radius
#define width_minus_dice screen_width - dice_radius
#define width_minus_egg screen_width - egg_radius

int sill_texture_x = 0;
int sill_texture_y = 0;
bool has_sill_texture = false;

static object dice = {0};
static object egg = {0};
static float delta_time = 0.0f;
static bool in_collision = false;

static void initial_variables()
{
    dice.position = (Vector2){screen_width / 2.0f - 90.0f,
                              screen_height / 2.0f - 70.0f};
    dice.speed = (Vector2){-200.0f, 140.0f};
    dice.rotation_speed = 20.0f;
    dice.normal_color = DARKGREEN;
    dice.collide_color = GREEN;
    egg.position = (Vector2){screen_width / 2.0f + 100.0f,
                             screen_height / 2.0f - 10.0f};
    egg.speed = (Vector2){0.0f, 0.0f};
    egg.rotation_speed = 0.0f;
    egg.normal_color = GOLD;
    egg.collide_color = YELLOW;
}

static bool dice_out_of_bounds_x(float x)
{
    return x <= dice_radius || x >= width_minus_dice;
}

static bool dice_out_of_bounds_y(float y)
{
    return y <= dice_radius || y >= height_minus_dice;
}

static bool egg_out_of_bounds_x(float x)
{
    return x <= egg_radius || x >= width_minus_egg;
}

static bool egg_out_of_bounds_y(float y)
{
    return y <= egg_radius || y >= height_minus_egg;
}

static void update_dice()
{
    if (dice.speed.x == 0.0f && dice.speed.y == 0.0f)
        return;

    dice.position.x += dice.speed.x * delta_time;
    dice.position.y += dice.speed.y * delta_time;
    if (!in_collision)
        dice.rotation += dice.rotation_speed * delta_time;

    if (dice_out_of_bounds_x(dice.position.x))
    {
        if (dice.position.x < dice_radius)
            dice.position.x = dice_radius + 1.0f;
        else if (dice.position.x > width_minus_dice)
            dice.position.x = width_minus_dice - 1.0f;

        dice.speed.x *= -0.99f;
        dice.rotation_speed *= -0.99f;
    }

    if (dice_out_of_bounds_y(dice.position.y))
    {
        if (dice.position.y < dice_radius)
            dice.position.y = dice_radius + 1.0f;
        else if (dice.position.y > height_minus_dice)
            dice.position.y = height_minus_dice - 1.0f;

        dice.speed.y *= -0.99f;
        dice.rotation_speed *= -0.99f;
    }
}

static void update_egg()
{
    if (egg.speed.x == 0.0f && egg.speed.y == 0.0f)
        return;

    egg.position.x += egg.speed.x * delta_time;
    egg.position.y += egg.speed.y * delta_time;
    if (!in_collision)
        egg.rotation += egg.rotation_speed * delta_time;

    if (egg_out_of_bounds_x(egg.position.x))
    {
        if (egg.position.x < egg_radius)
            egg.position.x = egg_radius + 1.0f;
        else if (egg.position.x > width_minus_egg)
            egg.position.x = width_minus_egg - 1.0f;

        egg.speed.x *= -0.99f;
        egg.rotation_speed *= -0.99f;
    }

    if (egg_out_of_bounds_y(egg.position.y))
    {
        if (egg.position.y < egg_radius)
            egg.position.y = egg_radius + 1.0f;
        else if (egg.position.y > height_minus_egg)
            egg.position.y = height_minus_egg - 1.0f;

        egg.speed.y *= -0.99f;
        egg.rotation_speed *= -0.99f;
    }
}

static void update_variables()
{
    delta_time = GetFrameTime();
    update_dice();
    update_egg();
    Rectangle dice_rec;
    dice_rec.x = dice.position.x;
    dice_rec.width = dice_width;
    dice_rec.y = dice.position.y;
    dice_rec.height = dice_width;
    Rectangle egg_rec;
    egg_rec.x = egg.position.x;
    egg_rec.width = 2.0f * egg_width * 1.25f;
    egg_rec.y = egg.position.y;
    egg_rec.height = 2.0f * egg_width * 0.75f;
    if (!CheckCollisionRecs(dice_rec, egg_rec))
    {
        in_collision = false;
        return;
    }

    if (in_collision)
        return;

    in_collision = true;
    float esx = egg.speed.x;
    float esy = egg.speed.y;
    bool same_direction_x = (dice.speed.x > 0.0f && egg.speed.x > 0.0f) ||
                            (dice.speed.x < 0.0 && egg.speed.x < 0.0f);
    bool same_direction_y = (dice.speed.y > 0.0f && egg.speed.y > 0.0f) ||
                            (dice.speed.y < 0.0 && egg.speed.y < 0.0f);
    bool egg_faster_x = egg.speed.x > dice.speed.x;
    bool egg_faster_y = egg.speed.y > dice.speed.y;
    if (esx == 0.0f)
        egg.speed.x = dice.speed.x * 1.2f;
    else
    {
        if (same_direction_x)
        {
            if (!egg_faster_x)
                egg.speed.x += dice.speed.x * 0.2f;
            else
                egg.speed.x *= 0.9f;
        }
        else
        {
            egg.speed.x *= -0.9f;
        }
    }

    if (esy == 0.0f)
        egg.speed.y = dice.speed.y * 1.2f;
    else
    {
        if (same_direction_y)
        {
            if (!egg_faster_y)
                egg.speed.y += dice.speed.y * 0.2f;
            else
                egg.speed.y *= 0.9f;
        }
        else
            egg.speed.y *= -0.9f;
    }

    if (esx == 0.0f)
        dice.speed.x *= 0.9f;
    else
    {
        if (same_direction_x)
        {
            if (egg_faster_x)
                dice.speed.x += esx * 0.1f;
            else
                dice.speed.x *= 0.9f;
        }
        else
            dice.speed.x *= -0.9f;
    }

    if (esy == 0.0f)
        dice.speed.y *= 0.9f;
    else
    {
        if (same_direction_y)
        {
            if (egg_faster_y)
                dice.speed.y += esy * 0.1f;
            else
                dice.speed.y *= 0.9f;
        }
        else
            dice.speed.y *= -0.9f;
    }

    if (esx == 0.0f || esy == 0.0f)
        dice.rotation_speed *= 0.9f;
    else
        dice.rotation_speed *= 1.1f;

    if (egg.rotation_speed == 0.0f)
        egg.rotation_speed = 5.0f;
    else
        egg.rotation_speed *= 1.1f;
}

static void draw_dice()
{
    Color color = dice.normal_color;
    if (in_collision)
        color = dice.collide_color;
    if (dice.rotation > 360.0f)
        dice.rotation = 1.0f;
    else if (dice.rotation < 0.0f)
        dice.rotation = 359.0;
    DrawRectanglePro(
        (Rectangle){dice.position.x, dice.position.y, dice_width, dice_width},
        (Vector2){dice_radius, dice_radius}, dice.rotation, color);
}

static void draw_egg()
{
    Color color = egg.normal_color;
    if (in_collision)
        color = egg.collide_color;
    if (egg.rotation != 0.0f)
    {
        rlPushMatrix();
        {
            rlTranslatef(egg.position.x, egg.position.y, 0.0f);
            if (egg.rotation > 360.0f)
                egg.rotation = 1.0f;
            else if (egg.rotation < 0.0f)
                egg.rotation = 359.0;
            rlRotatef(egg.rotation, 0.0f, 0.0f, 1.0f);
            DrawEllipse(0.0f, 0.0f,
                        egg_width * 1.25f, egg_width * 0.75f, color);
        }
        rlPopMatrix();
    }
    else
        DrawEllipse(egg.position.x, egg.position.y,
                    egg_width * 1.25f, egg_width * 0.75f, color);
}

static void draw_objects()
{
    draw_egg();
    draw_dice();
}

static void draw_text()
{
    DrawText("Press escape to exit", 190, 200, 20, LIGHTGRAY);
}

static void draw_contents()
{
    DrawTexture(sill_texture, sill_texture_x, sill_texture_y, WHITE);
    draw_objects();
    draw_text();
}

static void draw_contents_no_texture()
{
    draw_objects();
    draw_text();
}

static void draw()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_contents();
    DrawFPS(10, 10);
    EndDrawing();
}

static void draw_no_texture()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    draw_contents_no_texture();
    DrawFPS(10, 10);
    EndDrawing();
}

void main_loop()
{
    initial_variables();
    if (!has_sill_texture)
    {
        while (!WindowShouldClose())
        {
            update_variables();
            draw_no_texture();
        }

        return;
    }

    while (!WindowShouldClose())
    {
        update_variables();
        draw();
    }
}
