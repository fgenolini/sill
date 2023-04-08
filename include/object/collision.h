#ifndef __OBJECT_COLLISION_H__
#define __OBJECT_COLLISION_H__

#include "object/object.h"

#define collision_count object_count *(object_count - 1) / 2

struct collision_state_t
{
    object *end[2];
    Vector2 speed_after[2];
    float rotation_speed_after[2];
    int id[2];
    bool a_faster_x;
    bool a_faster_y;
    bool same_direction_x;
    bool same_direction_y;
    bool collision_detected;
    bool in_collision;
};
typedef struct collision_state_t collision_state;

extern collision_state collisions[collision_count];

// Creaate a collision for each pair of objects
extern void initialise_collision_pairs();

// Location change, speed, rotation, collision
extern void update_objects();

extern Vector2 speed_after_collision(const object *this_end,
                                     int collision_id, int other_end,
                                     float *rotation_speed);
extern bool is_in_collision(int object_id);

#endif
