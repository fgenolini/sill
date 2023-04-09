#include <math.h>
#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

#include "object/collision.h"
#include "object/object.h"
#include "say.h"

collision_state collisions[collision_count] = {0};

static Rectangle rectangles[object_count] = {0};
static char buffer[256];
static int count = 0;

void initialise_collision_pairs()
{
    const char *func = "initialise_collision_pairs";
    if (collision_count < 1)
        return;

    sprintf(buffer, "%d pairs", collision_count);
    say(__FILE__, func, __LINE__, LOG_INFO, buffer);
    int count = 0;
    for (int a = 0; a < object_count; ++a)
    {
        if (count >= collision_count)
            break;

        for (int b = a + 1; b < object_count; ++b)
        {
            if (count >= collision_count)
                break;

            collisions[count].end[0] = objects[a];
            collisions[count].end[1] = objects[b];
            collisions[count].id[0] = a;
            collisions[count].id[1] = b;
            collisions[count].collision_detected = false;
            collisions[count].in_collision = false;
            ++count;
        }
    }
}

static void handle_collisions()
{
    const char *func = "handle_collisions";
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
            c->rotation_speed_after[0] = 0.0f;
            c->rotation_speed_after[1] = 0.0f;
            continue;
        }

        any_collision = true;
        object *e0 = c->end[0];
        object *e1 = c->end[1];
        c->in_collision = true;
        float a_speed_x = e0->speed.x;
        float b_speed_x = e1->speed.x;
        c->same_direction_x = (a_speed_x >= 0.0f && b_speed_x >= 0.0f) ||
                              (a_speed_x < 0.0f && b_speed_x < 0.0f);
        float a_speed_y = e0->speed.y;
        float b_speed_y = e1->speed.y;
        c->same_direction_y = (a_speed_y >= 0.0f && b_speed_y >= 0.0f) ||
                              (a_speed_y < 0.0f && b_speed_y < 0.0f);
        c->a_faster_x = fabs(a_speed_x) > fabs(b_speed_x);
        c->a_faster_y = fabs(a_speed_y) > fabs(b_speed_y);
        c->speed_after[0] = e0->speed_after(e0, i, 1,
                                            &c->rotation_speed_after[0]);
        c->speed_after[1] = e1->speed_after(e1, i, 0,
                                            &c->rotation_speed_after[1]);
    }

    if (!any_collision)
        return;

    ++count;
    for (int i = 0; i < object_count; ++i)
    {
        int speed_count = 0;
        Vector2 speed = {0};
        float rotation_speed = 0.0f;
        for (int j = 0; j < collision_count; j++)
        {
            collision_state *c = &collisions[j];
            if (!c->collision_detected)
                continue;

            if (c->id[0] != i && c->id[1] != i)
                continue;

            Vector2 speed_after = {0};
            float rotation_speed_after = 0.0f;
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

            if (speed_after.x == 0.0f && speed_after.y == 0.0f)
                continue;

            ++speed_count;
            if (speed_count == 1)
            {
                speed = speed_after;
                rotation_speed = rotation_speed_after;
                continue;
            }

            // Multiple collisions, multiple speeds
            speed = Vector2Add(speed, speed_after);
            rotation_speed += rotation_speed_after;
        }

        if (!speed_count)
            continue;

        objects[i]->speed = speed;
        objects[i]->rotation_speed = rotation_speed;
        if (speed_count > 1)
            say(__FILE__, func, __LINE__, LOG_DEBUG, "multiple speeds");
    }
}

void update_objects_after_collision()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->update(objects[i]);

    for (int i = 0; i < object_count; ++i)
        rectangles[i] = objects[i]->collision_rectangle(objects[i]);

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

Vector2 speed_after_collision(const object *this_end, int collision_id,
                              int other_end, float *rotation_speed)
{
    Vector2 speed_after = {0};
    const collision_state *c = &collisions[collision_id];
    const object *other = c->end[other_end];
    const float sx = other->speed.x;
    const bool this_has_speed_x = fabs(this_end->speed.x) > 0.1f;
    const bool other_has_speed_x = fabs(sx) > 0.1f;
    const float sy = other->speed.y;
    const bool this_has_speed_y = fabs(this_end->speed.y) > 0.1f;
    const bool other_has_speed_y = fabs(sy) > 0.1f;
    if (fabs(*rotation_speed) < 0.1f)
        *rotation_speed = 15.0f;

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

    bool same_speed_x = fabs(sx - this_end->speed.x) < 1.0f;
    bool i_am_faster_x = c->a_faster_x;
    if (other_end == 0)
        i_am_faster_x = !i_am_faster_x;

    if (!other_has_speed_x)
        speed_after.x = this_end->speed.x * 0.9f;
    else
    {
        if (c->same_direction_x)
        {
            if (same_speed_x)
                speed_after.x = this_end->speed.x;
            else if (i_am_faster_x)
                speed_after.x = this_end->speed.x * 0.8f;
            else
                speed_after.x = this_end->speed.x + sx * 0.2f;
        }
        else
            speed_after.x = this_end->speed.x * -0.9f;
    }

    bool same_speed_y = fabs(sy - this_end->speed.y) < 1.0f;
    bool i_am_faster_y = c->a_faster_y;
    if (other_end == 0)
        i_am_faster_y = !i_am_faster_y;

    if (!other_has_speed_y)
        speed_after.y = this_end->speed.y * 0.9f;
    else
    {
        if (c->same_direction_y)
        {
            if (same_speed_y)
                speed_after.y = this_end->speed.y;
            else if (i_am_faster_y)
                speed_after.y = this_end->speed.y * 0.8f;
            else
                speed_after.y = this_end->speed.y + sy * 0.2f;
        }
        else
            speed_after.y = this_end->speed.y * -0.9f;
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
