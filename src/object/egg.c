#include <math.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "init.h"
#include "load_resources.h"
#include "object/collision.h"
#include "object/egg.h"
#include "say.h"

enum egg_internal_constant
{
    buffer_length = 16,

    /// @brief radius of the circle before h_factor and v_factor deformation
    egg_radius = 20
};

/// @brief ellipse horizontal deformation
static const float h_factor = 1.1f;

/// @brief ellipse vertical deformation
static const float v_factor = 0.9f;

static char buffer[buffer_length];

static void update_egg(object *me)
{
    update_object(me);
}

static Rectangle egg_collision_rectangle(const object *me)
{
    static const float extra = 1.5f;
    Rectangle egg_rec = {0};
    egg_rec.x = me->position.x - extra;
    egg_rec.width = 2 * me->radius * 2 + 2 * extra;
    egg_rec.y = me->position.y - extra;
    egg_rec.height = 2 * me->radius * 2 + 2 * extra;
    return egg_rec;
}

static Vector2 egg_speed_after(const object *me,
                               int collision_id, int other_end,
                               float *rotation_speed)
{
    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_egg(const object *me)
{
    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;

    Vector2 position = me->position;
    if (me->rotation != 0)
    {
        rlPushMatrix();
        {
            rlTranslatef(position.x, position.y, 0);
            float rotation = fmod(me->rotation, 360);
            if (rotation < 0)
                rotation += 360;

            rlRotatef(rotation, 0, 0, 1);
            DrawEllipse(0, 0,
                        me->radius * 2 * h_factor,
                        me->radius * 2 * v_factor, color);
        }
        rlPopMatrix();
    }
    else
        DrawEllipse(position.x, position.y,
                    me->radius * 2 * h_factor,
                    me->radius * 2 * v_factor, color);
}

static void init_egg_instance(int object_id, object *optional_instance)
{
    static const char *func = "init_egg_instance";
    if (!optional_instance || optional_instance == &egg)
    {
        snprintf(buffer, buffer_length, "my egg:%d", egg.id);
        say(__FILE__, func, __LINE__, LOG_INFO, buffer);
        return;
    }

    *optional_instance = egg;
    optional_instance->id = object_id;
    snprintf(buffer, buffer_length, "egg_%08d", object_id);
    set_object_name(object_id, buffer);
    optional_instance->normal_color.a = 200;
    optional_instance->collide_color.a = 130;
    snprintf(buffer, buffer_length, "egg:%d", object_id);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
}

static const method_table egg_methods = {
    init_egg_instance,
    draw_egg,
    update_egg,
    egg_speed_after,
    egg_collision_rectangle,
};

const object egg = {
    &egg_methods,

    // ID (-1 = class, not object instance)
    -1,

    // collide color
    YELLOW,

    // normal (non colliding) color
    GOLD,

    // position
    (Vector2){screen_width / 2 + 100, screen_height / 2 - 10},

    // speed
    (Vector2){0, 0},

    egg_radius,

    // rotation
    0,

    // rotation speed
    0,
};
