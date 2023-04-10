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

static void update_egg(object *optional_instance)
{
    object *me = optional_instance;
    if (!me)
        // cannot update class (only object instances)
        return;

    update_object(me);
}

static Rectangle egg_collision_rectangle(const object *optional_instance)
{
    const object *me = optional_instance;
    if (!me)
        me = &egg;

    static const float extra = 1.5f;
    Rectangle egg_rec = {0};
    egg_rec.x = me->position.x - extra;
    egg_rec.width = 2.0f * me->radius * 2.0f + 2.0f * extra;
    egg_rec.y = me->position.y - extra;
    egg_rec.height = 2.0f * me->radius * 2.0f + 2.0f * extra;
    return egg_rec;
}

static Vector2 egg_speed_after(const object *optional_instance,
                               int collision_id, int other_end,
                               float *rotation_speed)
{
    const object *me = optional_instance;
    if (!me)
        me = &egg;

    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_egg(const object *optional_instance)
{
    const object *me = optional_instance;
    if (!me)
        // class cannot be drawn, only object instances can
        return;

    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;

    Vector2 position = me->position;
    if (me->rotation != 0)
    {
        rlPushMatrix();
        {
            rlTranslatef(position.x, position.y, 0);
            float rotation = fmod(me->rotation, 360.0);
            if (rotation < 0)
                rotation += 360.0f;

            rlRotatef(rotation, 0, 0, 1);
            DrawEllipse(0, 0,
                        me->radius * 2.0f * h_factor,
                        me->radius * 2.0f * v_factor, color);
        }
        rlPopMatrix();
    }
    else
        DrawEllipse(position.x, position.y,
                    me->radius * 2.0f * h_factor,
                    me->radius * 2.0f * v_factor, color);
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
    snprintf(object_names[object_id], object_name_length,
             "egg_%08d", object_id);
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
    (Vector2){screen_width / 2.0f + 100.0f,
              screen_height / 2.0f - 10.0f},
    (Vector2){0, 0},
    GOLD,
    YELLOW,
    0,
    0,
    egg_radius,
    0};
