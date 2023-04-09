#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "init.h"
#include "load_resources.h"
#include "object/collision.h"
#include "object/dice.h"
#include "object/egg.h"
#include "object/object.h"

static object dice1 = {0};
static object egg1 = {0};
static object egg2 = {0};
static object green_stone1 = {0};

object *objects[object_count] = {&dice1, &egg1, &egg2, &green_stone1};

bool out_of_bounds(const object *me)
{
    return out_of_bounds_x(me) || out_of_bounds_y(me);
}

bool out_of_bounds_x(const object *me)
{
    return me->position.x < me->radius ||
           me->position.x > screen_width - me->radius;
}

bool out_of_bounds_y(const object *me)
{
    return me->position.y < me->radius ||
           me->position.y > screen_height - me->radius;
}

bool really_out_of_bounds(const object *me)
{
    return me->position.x < -me->radius ||
           me->position.y < -me->radius ||
           me->position.x > screen_width + me->radius ||
           me->position.y > screen_height + me->radius;
}

void update_object(object *me)
{
    me->position.x += me->speed.x * main_state.delta_time;
    me->position.y += me->speed.y * main_state.delta_time;
    if (!is_in_collision(me->id))
    {
        me->rotation += me->rotation_speed * main_state.delta_time;
        me->rotation = fmod(me->rotation, 360.0);
        if (me->rotation < 0)
            me->rotation += 360.0f;
    }

    if (!out_of_bounds(me))
    {
        me->last_good_position = me->position;
        return;
    }

    if (fabs(me->rotation_speed) < 10.0f)
        me->rotation_speed = 10.0f;
    else
        me->rotation_speed *= 1.01f;

    if (out_of_bounds_x(me))
    {
        const float left_edge = me->radius + 1.0f;
        const float right_edge = screen_width - me->radius - 1.0f;
        const bool positive_speed = me->speed.x > 0.0f;
        if (me->position.x < left_edge)
        {
            me->position.x += 4.0f;
            if (me->position.x < -me->radius)
                me->position.x = 1.0f;

            if (!positive_speed)
                me->speed.x *= -1.0f;
        }
        else if (me->position.x > right_edge)
        {
            me->position.x -= 4.0f;
            if (me->position.x > screen_width + me->radius)
                me->position.x = screen_width - 1.0f;

            if (positive_speed)
                me->speed.x *= -1.0f;
        }

        me->speed.x *= 0.99f;
    }
    else
        me->last_good_position.x = me->position.x;

    if (out_of_bounds_y(me))
    {
        const float bottom_edge = screen_height - me->radius - 1.0f;
        const float top_edge = me->radius + 1.0f;
        const bool positive_speed = me->speed.y > 0.0f;
        if (me->position.y < top_edge)
        {
            me->position.y += 4.0f;
            if (me->position.y < -me->radius)
                me->position.y = 1.0f;

            if (!positive_speed)
                me->speed.y *= -1.0f;
        }
        else if (me->position.y > bottom_edge)
        {
            me->position.y -= 4.0f;
            if (me->position.y > screen_height + me->radius)
                me->position.y = screen_height - 1.0f;

            if (positive_speed)
                me->speed.y *= -1.0f;
        }

        me->speed.y *= 0.99f;
    }
    else
        me->last_good_position.y = me->position.y;
}

void initialise_objects()
{
    dice1.class_type = &dice;
    egg1.class_type = &egg;
    egg2.class_type = &egg;
    green_stone1.class_type = &dice;
    for (int i = 0; i < object_count; ++i)
        objects[i]->class_type->init(i, objects[i]);

    egg1.rotation = 350.0f;
    egg2.rotation = 355.0f;
    egg2.position = (Vector2){egg1.position.x + 120.0f,
                              egg1.position.y + 40.0f};
    green_stone1.position = (Vector2){egg2.position.x - 210.0f,
                                      egg2.position.y + 50.0f};
    green_stone1.speed = (Vector2){0};
    green_stone1.radius = dice1.radius / 2.5f;
    green_stone1.rotation = 240.0f;
    green_stone1.rotation_speed = 0.0f;
}

void draw_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->draw(objects[i]);
}
