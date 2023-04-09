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

static void update_dice(object *optional_instance)
{
    object *me = &dice;
    if (optional_instance)
        me = optional_instance;

    if (me->speed.x == 0.0f && me->speed.y == 0.0f)
        return;

    me->position.x += me->speed.x * main_state.delta_time;
    me->position.y += me->speed.y * main_state.delta_time;
    if (!is_in_collision(me->id))
        me->rotation += me->rotation_speed * main_state.delta_time;

    if (dice_out_of_bounds_x(me->position.x))
    {
        if (me->position.x < dice_radius)
            me->position.x = dice_radius + 1.0f;
        else if (me->position.x > width_minus_dice)
            me->position.x = width_minus_dice - 1.0f;

        me->speed.x *= -0.99f;
        me->rotation_speed *= -0.99f;
    }

    if (dice_out_of_bounds_y(me->position.y))
    {
        if (me->position.y < dice_radius)
            me->position.y = dice_radius + 1.0f;
        else if (me->position.y > height_minus_dice)
            me->position.y = height_minus_dice - 1.0f;

        me->speed.y *= -0.99f;
        me->rotation_speed *= -0.99f;
    }
}

static Rectangle dice_collision_rectangle(object *optional_instance)
{
    object *me = &dice;
    if (optional_instance)
        me = optional_instance;

    Rectangle dice_rec = {0};
    dice_rec.x = me->position.x;
    dice_rec.width = dice_width;
    dice_rec.y = me->position.y;
    dice_rec.height = dice_width;
    return dice_rec;
}

static Vector2 dice_speed_after(object *optional_instance,
                                int collision_id, int other_end,
                                float *rotation_speed)
{
    object *me = &dice;
    if (optional_instance)
        me = optional_instance;

    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_dice(object *optional_instance)
{
    object *me = &dice;
    if (optional_instance)
        me = optional_instance;

    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;
    if (me->rotation > 360.0f)
        me->rotation = 1.0f;
    else if (me->rotation < 0.0f)
        me->rotation = 359.0;
    DrawRectanglePro(
        (Rectangle){me->position.x, me->position.y, dice_width, dice_width},
        (Vector2){dice_radius, dice_radius}, me->rotation, color);
}

static void init_dice_instance(int object_id, object *optional_instance)
{
    const char *func = "init_dice_instance";
    if (!optional_instance || optional_instance == &dice)
    {
        dice.id = object_id;
        sprintf(buffer, "my dice:%d", dice.id);
        say(__FILE__, func, __LINE__, LOG_INFO, buffer);
        return;
    }

    *optional_instance = dice;
    optional_instance->class_type = &dice;
    optional_instance->id = object_id;
    sprintf(buffer, "dice:%d", object_id);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
}

object dice = {
    init_dice_instance,
    draw_dice,
    update_dice,
    dice_speed_after,
    dice_collision_rectangle,
    NULL,
    (Vector2){screen_width / 2.0f - 90.0f,
              screen_height / 2.0f - 70.0f},
    (Vector2){-200.0f, 140.0f},
    DARKGREEN,
    GREEN,
    0.0f,
    20.0f,
    0,
    "dice"};
