#include <stdio.h>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "init.h"
#include "load_resources.h"
#include "object/collision.h"
#include "object/egg.h"
#include "say.h"

#define egg_radius 20.0f
#define egg_width egg_radius * 2.0f
#define height_minus_egg screen_height - egg_radius
#define width_minus_egg screen_width - egg_radius

static char buffer[256];

static bool egg_out_of_bounds_x(float x)
{
    return x <= egg_radius || x >= width_minus_egg;
}

static bool egg_out_of_bounds_y(float y)
{
    return y <= egg_radius || y >= height_minus_egg;
}

static void update_egg(object *optional_instance)
{
    object *me = &egg;
    if (optional_instance)
        me = optional_instance;

    if (me->speed.x == 0.0f && me->speed.y == 0.0f)
        return;

    me->position.x += me->speed.x * main_state.delta_time;
    me->position.y += me->speed.y * main_state.delta_time;
    if (!is_in_collision(me->id))
        me->rotation += me->rotation_speed * main_state.delta_time;

    if (egg_out_of_bounds_x(me->position.x))
    {
        if (me->position.x < egg_radius)
            me->position.x = egg_radius + 1.0f;
        else if (me->position.x > width_minus_egg)
            me->position.x = width_minus_egg - 1.0f;

        me->speed.x *= -0.99f;
        me->rotation_speed *= -0.99f;
    }

    if (egg_out_of_bounds_y(me->position.y))
    {
        if (me->position.y < egg_radius)
            me->position.y = egg_radius + 1.0f;
        else if (me->position.y > height_minus_egg)
            me->position.y = height_minus_egg - 1.0f;

        me->speed.y *= -0.99f;
        me->rotation_speed *= -0.99f;
    }
}

static Rectangle egg_collision_rectangle(object *optional_instance)
{
    object *me = &egg;
    if (optional_instance)
        me = optional_instance;

    Rectangle egg_rec = {0};
    egg_rec.x = me->position.x;
    egg_rec.width = 2.0f * egg_width * 1.25f;
    egg_rec.y = me->position.y;
    egg_rec.height = 2.0f * egg_width * 0.75f;
    return egg_rec;
}

static Vector2 egg_speed_after(object *optional_instance,
                               int collision_id, int other_end,
                               float *rotation_speed)
{
    object *me = &egg;
    if (optional_instance)
        me = optional_instance;

    float rotation_speed_after = me->rotation_speed;
    Vector2 speed_after = speed_after_collision(me, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_egg(object *optional_instance)
{
    object *me = &egg;
    if (optional_instance)
        me = optional_instance;

    Color color = me->normal_color;
    if (is_in_collision(me->id))
        color = me->collide_color;

    if (me->rotation != 0.0f)
    {
        rlPushMatrix();
        {
            rlTranslatef(me->position.x, me->position.y, 0.0f);
            if (me->rotation > 360.0f)
                me->rotation = 1.0f;
            else if (me->rotation < 0.0f)
                me->rotation = 359.0;
            rlRotatef(me->rotation, 0.0f, 0.0f, 1.0f);
            DrawEllipse(0.0f, 0.0f,
                        egg_width * 1.25f, egg_width * 0.75f, color);
        }
        rlPopMatrix();
    }
    else
        DrawEllipse(me->position.x, me->position.y,
                    egg_width * 1.25f, egg_width * 0.75f, color);
}

static void init_egg_instance(int object_id, object *optional_instance)
{
    const char *func = "init_egg_instance";
    if (!optional_instance || optional_instance == &egg)
    {
        egg.id = object_id;
        sprintf(buffer, "my egg:%d", egg.id);
        say(__FILE__, func, __LINE__, LOG_INFO, buffer);
        return;
    }

    *optional_instance = egg;
    optional_instance->id = object_id;
    sprintf(buffer, "egg:%d", object_id);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
}

object egg = {
    init_egg_instance,
    draw_egg,
    update_egg,
    egg_speed_after,
    egg_collision_rectangle,
    (Vector2){screen_width / 2.0f + 100.0f,
              screen_height / 2.0f - 10.0f},
    (Vector2){0.0f, 0.0f},
    GOLD,
    YELLOW,
    0.0f,
    0.0f,
    0,
    "egg"};
