#ifndef __OBJECT_OBJECT_H__
#define __OBJECT_OBJECT_H__

#include "raylib.h"

#define object_count 4
#define object_name_length 60

struct object_t
{
    void (*init)(int object_id, struct object_t *optional_instance);
    void (*draw)(const struct object_t *optional_instance);
    void (*update)(struct object_t *optional_instance);
    Vector2 (*speed_after)(const struct object_t *optional_instance,
                           int collision_id, int other_end,
                           float *rotation_speed);
    Rectangle (*collision_rectangle)(const struct object_t *optional_instance);

    struct object_t *class_type;
    Vector2 position;
    Vector2 last_good_position;
    Vector2 speed;
    Color normal_color;
    Color collide_color;
    float rotation;
    float rotation_speed;
    float radius;
    int id;
    char name[object_name_length];
};
typedef struct object_t object;

extern object *objects[object_count];

extern void initialise_objects();
extern void draw_objects();
extern bool out_of_bounds(const object *me);
extern bool out_of_bounds_x(const object *me);
extern bool out_of_bounds_y(const object *me);
extern bool really_out_of_bounds(const object *me);
extern void update_object(object *me);

#endif
