#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "init.h"
#include "load_resources.h"
#include "object/collision.h"
#include "object/dice.h"
#include "say.h"

#define dice_radius 50.0f
#define dice_width dice_radius * 2.0f
#define height_minus_dice screen_height - dice_radius
#define width_minus_dice screen_width - dice_radius

static char buffer[256];

static bool dice_out_of_bounds_x(float x)
{
    return x <= dice_radius || x >= width_minus_dice;
}

static bool dice_out_of_bounds_y(float y)
{
    return y <= dice_radius || y >= height_minus_dice;
}

static void update_dice()
{
    if (dice.speed.x == 0.0f && dice.speed.y == 0.0f)
        return;

    dice.position.x += dice.speed.x * main_state.delta_time;
    dice.position.y += dice.speed.y * main_state.delta_time;
    if (!is_in_collision(dice.id))
        dice.rotation += dice.rotation_speed * main_state.delta_time;

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

static Rectangle dice_collision_rectangle()
{
    Rectangle dice_rec = {0};
    dice_rec.x = dice.position.x;
    dice_rec.width = dice_width;
    dice_rec.y = dice.position.y;
    dice_rec.height = dice_width;
    return dice_rec;
}

static Vector2 dice_speed_after(int collision_id, int other_end,
                                float *rotation_speed)
{
    float rotation_speed_after = dice.rotation_speed;
    Vector2 speed_after = speed_after_collision(&dice, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_dice()
{
    Color color = dice.normal_color;
    if (is_in_collision(dice.id))
        color = dice.collide_color;
    if (dice.rotation > 360.0f)
        dice.rotation = 1.0f;
    else if (dice.rotation < 0.0f)
        dice.rotation = 359.0;
    DrawRectanglePro(
        (Rectangle){dice.position.x, dice.position.y, dice_width, dice_width},
        (Vector2){dice_radius, dice_radius}, dice.rotation, color);
}

static void init_dice_instance(int object_id)
{
    const char *func = "init_dice_instance";
    dice.id = object_id;
    sprintf(buffer, "dice:%d", dice.id);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
}

object dice = {
    init_dice_instance,
    draw_dice,
    update_dice,
    dice_speed_after,
    dice_collision_rectangle,
    (Vector2){screen_width / 2.0f - 90.0f,
              screen_height / 2.0f - 70.0f},
    (Vector2){-200.0f, 140.0f},
    DARKGREEN,
    GREEN,
    0.0f,
    20.0f,
    0,
    "dice"};
