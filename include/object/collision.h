// basic collision handling, assuming point objects

#ifndef __OBJECT_COLLISION_H__
#define __OBJECT_COLLISION_H__

#include "raylib.h"

#include "object/object.h"

// Create a collision for each pair of objects
extern void initialise_collision_pairs();

extern bool is_in_collision(int object_id);

extern Vector2 speed_after_collision(const object *me,
                                     int collision_id, int other_end,
                                     float *rotation_speed);

// Location change, speed, rotation, collision
extern void update_objects_after_collision();

#endif
