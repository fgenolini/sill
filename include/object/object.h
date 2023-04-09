#ifndef __OBJECT_OBJECT_H__
#define __OBJECT_OBJECT_H__

#include "raylib.h"

#define object_count 3
#define object_name_length 60

struct object_t
{
    void (*init)(int object_id, struct object_t *optional_instance);
    void (*draw)(struct object_t *optional_instance);
    void (*update)(struct object_t *optional_instance);
    Vector2 (*speed_after)(struct object_t *optional_instance,
                           int collision_id, int other_end,
                           float *rotation_speed);
    Rectangle (*collision_rectangle)(struct object_t *optional_instance);

    struct object_t *class_type;
    Vector2 position;
    Vector2 speed;
    Color normal_color;
    Color collide_color;
    float rotation;
    float rotation_speed;
    int id;
    char name[object_name_length];
};
typedef struct object_t object;

extern object *objects[object_count];

extern void initialise_objects();
extern void draw_objects();

#endif
