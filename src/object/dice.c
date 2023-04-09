#include <math.h>
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

static char buffer[256];

static void update_dice(object *optional_instance)
{
    object *me = optional_instance;
    if (!me)
        me = &dice;

    update_object(me);
}

static Rectangle dice_collision_rectangle(const object *optional_instance)
{
    const object *me = optional_instance;
    if (!me)
        me = &dice;

    float extra = 1.5f;
    if (fabs(me->rotation < 10.0f))
        extra = 0.0f;

    Rectangle dice_rec = {0};
    dice_rec.x = me->position.x - extra;
    dice_rec.width = me->radius * 2.0f + 2.0f * extra;
    dice_rec.y = me->position.y - extra;
    dice_rec.height = me->radius * 2.0f + 2.0f * extra;
    return dice_rec;
}

static Vector2 dice_speed_after(const object *optional_instance,
                                int collision_id, int other_end,
                                float *rotation_speed)
{
    const object *me = optional_instance;
    if (!me)
        me = &dice;

    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_dice(const object *optional_instance)
{
    const object *me = optional_instance;
    if (!me)
        me = &dice;

    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;

    float rotation = fmod(me->rotation, 360.0);
    if (rotation < 0)
        rotation += 360.0f;

    Vector2 position = me->position;
    if (really_out_of_bounds(me))
        position = me->last_good_position;

    DrawRectanglePro((Rectangle){position.x, position.y,
                                 me->radius * 2.0f, me->radius * 2.0f},
                     (Vector2){me->radius, me->radius}, rotation, color);
}

static void init_dice_instance(int object_id, object *optional_instance)
{
    static const char *func = "init_dice_instance";
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
    sprintf(optional_instance->name, "dice_%09d", object_id);
    optional_instance->normal_color.a = 200;
    optional_instance->collide_color.a = 130;
    optional_instance->last_good_position = optional_instance->position;
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
    {0},
    (Vector2){-200.0f, 140.0f},
    DARKGREEN,
    GREEN,
    0.0f,
    20.0f,
    dice_radius,
    0,
    "dice"};
