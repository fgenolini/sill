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

enum dice_internal_constant
{
    buffer_length = 16,

    /// @brief half the side of a square
    dice_radius = 50
};

static char buffer[buffer_length];

static void update_dice(object *me)
{
    update_object(me);
}

static Rectangle dice_collision_rectangle(const object *me)
{
    float extra = 1.5f;
    if (fabs(me->rotation < 10))
        extra = 0;

    Rectangle dice_rec = {0};
    dice_rec.x = me->position.x - extra;
    dice_rec.width = me->radius * 2 + 2 * extra;
    dice_rec.y = me->position.y - extra;
    dice_rec.height = me->radius * 2 + 2 * extra;
    return dice_rec;
}

static Vector2 dice_speed_after(const object *me,
                                int collision_id, int other_end,
                                float *rotation_speed)
{
    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_dice(const object *me)
{
    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;

    float rotation = fmod(me->rotation, 360);
    if (rotation < 0)
        rotation += 360;

    Vector2 position = me->position;
    DrawRectanglePro((Rectangle){position.x, position.y,
                                 me->radius * 2, me->radius * 2},
                     (Vector2){me->radius, me->radius}, rotation, color);
}

static void init_dice_instance(int object_id, object *optional_instance)
{
    static const char *func = "init_dice_instance";
    if (!optional_instance || optional_instance == &dice)
    {
        snprintf(buffer, buffer_length, "my dice:%d", dice.id);
        say(__FILE__, func, __LINE__, LOG_INFO, buffer);
        return;
    }

    *optional_instance = dice;
    optional_instance->id = object_id;
    snprintf(buffer, buffer_length, "dice_%08d", object_id);
    set_object_name(object_id, buffer);
    optional_instance->normal_color.a = 200;
    optional_instance->collide_color.a = 130;
    snprintf(buffer, buffer_length, "dice:%d", object_id);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
}

static const method_table dice_methods = {
    init_dice_instance,
    draw_dice,
    update_dice,
    dice_speed_after,
    dice_collision_rectangle,
};

const object dice = {
    &dice_methods,

    // ID (-1 = class, not object instance)
    -1,

    // collide color
    GREEN,

    // normal (non colliding) color
    DARKGREEN,

    // position
    (Vector2){screen_width / 2 - 90, screen_height / 2 - 70},

    // speed
    (Vector2){-200, 140},

    dice_radius,

    // rotation
    0,

    // rotation speed
    20,
};
