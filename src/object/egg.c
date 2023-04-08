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

static void update_egg()
{
    if (egg.speed.x == 0.0f && egg.speed.y == 0.0f)
        return;

    egg.position.x += egg.speed.x * main_state.delta_time;
    egg.position.y += egg.speed.y * main_state.delta_time;
    if (!is_in_collision(egg.id))
        egg.rotation += egg.rotation_speed * main_state.delta_time;

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

static Rectangle egg_collision_rectangle()
{
    Rectangle egg_rec = {0};
    egg_rec.x = egg.position.x;
    egg_rec.width = 2.0f * egg_width * 1.25f;
    egg_rec.y = egg.position.y;
    egg_rec.height = 2.0f * egg_width * 0.75f;
    return egg_rec;
}

static Vector2 egg_speed_after(int collision_id, int other_end,
                               float *rotation_speed)
{
    float rotation_speed_after = egg.rotation_speed;
    Vector2 speed_after = speed_after_collision(&egg, collision_id, other_end,
                                                &rotation_speed_after);
    *rotation_speed = rotation_speed_after;
    return speed_after;
}

static void draw_egg()
{
    Color color = egg.normal_color;
    if (is_in_collision(egg.id))
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

static void init_egg_instance(int object_id)
{
    const char *func = "init_egg_instance";
    egg.id = object_id;
    sprintf(buffer, "egg:%d", egg.id);
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
