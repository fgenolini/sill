// basic graphical object, such as a square or a circle

#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "init.h"
#include "load_resources.h"
#include "object/collision.h"
#include "object/dice.h"
#include "object/egg.h"
#include "object/object.h"

enum object_internal_constant
{
    /// @brief number of objects that can collide with each other
    object_count = 4,

    /// @brief maximum length of an object's name
    object_name_length = 16
};

static char object_names[object_count][object_name_length];
static object dice1 = {0};
static object egg1 = {0};
static object egg2 = {0};
static object green_stone1 = {0};
static object *objects[object_count] = {&dice1, &egg1, &egg2, &green_stone1};

int get_object_count()
{
    return object_count;
}

object *get_object(int id)
{
    return objects[id];
}

void set_object_name(int id, const char *name)
{
    snprintf(object_names[id], object_name_length, "%s", name);
}

void draw_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->m->draw(objects[i]);
}

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

void update_object(object *me)
{
    me->position.x += me->speed.x * main_state.delta_time;
    me->position.y += me->speed.y * main_state.delta_time;
    if (!is_in_collision(me->id))
    {
        me->rotation += me->rotation_speed * main_state.delta_time;
        me->rotation = fmod(me->rotation, 360);
        if (me->rotation < 0)
            me->rotation += 360;
    }

    if (!out_of_bounds(me))
        return;

    if (fabs(me->rotation_speed) < 10)
        me->rotation_speed = 10;
    else
        me->rotation_speed *= 1.01f;

    if (out_of_bounds_x(me))
    {
        const float left_edge = me->radius + 1;
        const float right_edge = screen_width - me->radius - 1;
        const bool positive_speed = me->speed.x > 0;
        if (me->position.x < left_edge)
        {
            me->position.x += 4;
            if (me->position.x < -me->radius)
                me->position.x = 1;

            if (!positive_speed)
                me->speed.x *= -1;
        }
        else if (me->position.x > right_edge)
        {
            me->position.x -= 4;
            if (me->position.x > screen_width + me->radius)
                me->position.x = screen_width - 1;

            if (positive_speed)
                me->speed.x *= -1;
        }

        me->speed.x *= 0.99f;
    }

    if (out_of_bounds_y(me))
    {
        const float bottom_edge = screen_height - me->radius - 1;
        const float top_edge = me->radius + 1;
        const bool positive_speed = me->speed.y > 0;
        if (me->position.y < top_edge)
        {
            me->position.y += 4;
            if (me->position.y < -me->radius)
                me->position.y = 1;

            if (!positive_speed)
                me->speed.y *= -1;
        }
        else if (me->position.y > bottom_edge)
        {
            me->position.y -= 4;
            if (me->position.y > screen_height + me->radius)
                me->position.y = screen_height - 1;

            if (positive_speed)
                me->speed.y *= -1;
        }

        me->speed.y *= 0.99f;
    }
}

void initialise_objects()
{
    const object *class_types[object_count];
    class_types[0] = &dice;
    class_types[1] = &egg;
    class_types[2] = &egg;
    class_types[3] = &dice;
    for (int i = 0; i < object_count; ++i)
        class_types[i]->m->init(i, objects[i]);

    egg1.rotation = 350;
    egg2.rotation = 355;
    egg2.position = (Vector2){egg1.position.x + 120,
                              egg1.position.y + 40};
    green_stone1.position = (Vector2){egg2.position.x - 210,
                                      egg2.position.y + 50};
    green_stone1.speed = (Vector2){0};
    green_stone1.radius = dice1.radius / 2.5f;
    green_stone1.rotation = 240;
    green_stone1.rotation_speed = 0;
}
