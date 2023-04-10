// basic collision handling, assuming point objects

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

#include "object/collision.h"
#include "object/object.h"
#include "say.h"

struct collision_state_t
{
    /// @brief object IDs for each end of the collision
    int id[2];

    Vector2 speed_after[2];
    float rotation_speed_after[2];
    bool a_faster_x;
    bool a_faster_y;
    bool same_direction_x;
    bool same_direction_y;
    bool collision_detected;
    bool in_collision;
};
typedef struct collision_state_t collision_state;

/// @brief number of collisions between all objects
static int collision_count = 0;

static collision_state *collisions = NULL;
static Rectangle *rectangles = NULL;

void initialise_collision_pairs()
{
    const int object_count = get_object_count();
    if (object_count < 2)
        return;

    collision_count = object_count * (object_count - 1) / 2;
    rectangles = malloc(object_count * sizeof(Rectangle));
    collisions = malloc(collision_count * sizeof(collision_state));
    int count = 0;
    for (int a = 0; a < object_count; ++a)
        for (int b = a + 1; b < object_count; ++b)
        {
            if (count >= collision_count)
                return;

            collisions[count].id[0] = a;
            collisions[count].id[1] = b;
            collisions[count].collision_detected = false;
            collisions[count].in_collision = false;
            ++count;
        }
}

/// @brief sum all the forces on point objects and update speed and rotation
static void update_collided_objects()
{
    const int object_count = get_object_count();
    for (int i = 0; i < object_count; ++i)
    {
        int speed_count = 0;
        Vector2 speed = {0};
        float rotation_speed = 0;
        for (int j = 0; j < collision_count; j++)
        {
            const collision_state *c = &collisions[j];
            if (!c->collision_detected)
                continue;

            if (c->id[0] != i && c->id[1] != i)
                continue;

            Vector2 speed_after = {0};
            float rotation_speed_after = 0;
            if (c->id[0] == i)
            {
                speed_after = c->speed_after[0];
                rotation_speed_after = c->rotation_speed_after[0];
            }
            else
            {
                speed_after = c->speed_after[1];
                rotation_speed_after = c->rotation_speed_after[1];
            }

            if (speed_after.x == 0 && speed_after.y == 0)
                continue;

            ++speed_count;
            if (speed_count == 1)
            {
                speed = speed_after;
                rotation_speed = rotation_speed_after;
                continue;
            }

            speed = Vector2Add(speed, speed_after);
            rotation_speed += rotation_speed_after;
        }

        if (!speed_count)
            continue;

        object *me = get_object(i);
        me->speed = speed;
        me->rotation_speed = rotation_speed;
    }
}

static void handle_collisions()
{
    bool any_collision = false;
    for (int i = 0; i < collision_count; ++i)
    {
        collision_state *c = &collisions[i];
        if (!c->collision_detected)
            continue;

        if (c->in_collision)
        {
            c->speed_after[0] = (Vector2){0};
            c->speed_after[1] = (Vector2){0};
            c->rotation_speed_after[0] = 0;
            c->rotation_speed_after[1] = 0;
            continue;
        }

        any_collision = true;
        const object *e0 = get_object(c->id[0]);
        const object *e1 = get_object(c->id[1]);
        c->in_collision = true;
        const float a_speed_x = e0->speed.x;
        const float b_speed_x = e1->speed.x;
        c->same_direction_x = (a_speed_x >= 0 && b_speed_x >= 0) ||
                              (a_speed_x < 0 && b_speed_x < 0);
        const float a_speed_y = e0->speed.y;
        const float b_speed_y = e1->speed.y;
        c->same_direction_y = (a_speed_y >= 0 && b_speed_y >= 0) ||
                              (a_speed_y < 0 && b_speed_y < 0);
        c->a_faster_x = fabs(a_speed_x) > fabs(b_speed_x);
        c->a_faster_y = fabs(a_speed_y) > fabs(b_speed_y);
        c->speed_after[0] = e0->m->speed_after(e0, i, 1,
                                               &c->rotation_speed_after[0]);
        c->speed_after[1] = e1->m->speed_after(e1, i, 0,
                                               &c->rotation_speed_after[1]);
    }

    if (!any_collision)
        return;

    update_collided_objects();
}

void update_objects_after_collision()
{
    const int object_count = get_object_count();
    for (int i = 0; i < object_count; ++i)
    {
        object *me = get_object(i);
        me->m->update(me);
    }

    for (int i = 0; i < object_count; ++i)
    {
        object *me = get_object(i);
        rectangles[i] = me->m->collision_rectangle(me);
    }

    bool any_collision = false;
    for (int i = 0; i < collision_count; ++i)
    {
        collision_state *c = &collisions[i];
        if (CheckCollisionRecs(rectangles[c->id[0]], rectangles[c->id[1]]))
        {
            c->collision_detected = true;
            any_collision = true;
        }
        else
        {
            c->collision_detected = false;
            c->in_collision = false;
        }
    }

    if (!any_collision)
        return;

    handle_collisions();
}

Vector2 speed_after_collision(const object *me, int collision_id,
                              int other_end, float *rotation_speed)
{
    Vector2 speed_after = {0};
    const collision_state *c = &collisions[collision_id];
    const object *other = get_object(c->id[other_end]);
    const float sx = other->speed.x;
    const bool this_has_speed_x = fabs(me->speed.x) > 1;
    const bool other_has_speed_x = fabs(sx) > 1;
    const float sy = other->speed.y;
    const bool this_has_speed_y = fabs(me->speed.y) > 1;
    const bool other_has_speed_y = fabs(sy) > 1;
    if (fabs(*rotation_speed) < 1)
        *rotation_speed = 15;

    if (!other_has_speed_x && !other_has_speed_y)
        *rotation_speed *= 0.9f;
    else
        *rotation_speed *= 1.1f;

    bool borrowed_x = false;
    if (other_has_speed_x && !this_has_speed_x)
    {
        speed_after.x = sx * 1.2f;
        borrowed_x = true;
    }

    bool borrowed_y = false;
    if (other_has_speed_y && !this_has_speed_y)
    {
        speed_after.y = sy * 1.2f;
        borrowed_y = true;
    }

    if (borrowed_x && borrowed_y)
        return speed_after;

    if (!other_has_speed_x)
        speed_after.x = me->speed.x * 0.9f;
    else
    {
        if (c->same_direction_x)
        {
            const bool same_speed_x = fabs(sx - me->speed.x) < 1;
            bool i_am_faster_x = c->a_faster_x;
            if (other_end == 0)
                i_am_faster_x = !i_am_faster_x;

            if (same_speed_x)
                speed_after.x = me->speed.x;
            else if (i_am_faster_x)
                speed_after.x = me->speed.x * 0.8f;
            else
                speed_after.x = me->speed.x + sx * 0.2f;
        }
        else
            speed_after.x = me->speed.x * -0.9f;
    }

    if (!other_has_speed_y)
        speed_after.y = me->speed.y * 0.9f;
    else
    {
        if (c->same_direction_y)
        {
            const bool same_speed_y = fabs(sy - me->speed.y) < 1;
            bool i_am_faster_y = c->a_faster_y;
            if (other_end == 0)
                i_am_faster_y = !i_am_faster_y;

            if (same_speed_y)
                speed_after.y = me->speed.y;
            else if (i_am_faster_y)
                speed_after.y = me->speed.y * 0.8f;
            else
                speed_after.y = me->speed.y + sy * 0.2f;
        }
        else
            speed_after.y = me->speed.y * -0.9f;
    }

    return speed_after;
}

bool is_in_collision(int object_id)
{
    for (int i = 0; i < collision_count; ++i)
    {
        const collision_state *c = &collisions[i];
        if (c->id[0] != object_id && c->id[1] != object_id)
            continue;

        if (c->in_collision)
            return true;
    }

    return false;
}
