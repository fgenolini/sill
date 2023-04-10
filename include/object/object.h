#ifndef __OBJECT_OBJECT_H__
#define __OBJECT_OBJECT_H__

#include "raylib.h"

enum object_constant
{
    /// @brief number of objects that can collide with each other
    object_count = 4,

    /// @brief maximum length of an object's name
    object_name_length = 16
};

struct object_t;
struct method_table_t
{
    /// @brief for classes only: initialise an object instance of that class
    void (*init)(int object_id, struct object_t *optional_instance);

    /// @brief draw the object at position with rotation
    void (*draw)(const struct object_t *optional_instance);

    /// @brief call update_object to set position, speed and rotation
    void (*update)(struct object_t *optional_instance);

    /// @brief estimate what the speed would be after a single collision
    Vector2 (*speed_after)(const struct object_t *optional_instance,
                           int collision_id, int other_end,
                           float *rotation_speed);

    /// @brief approximate the collision rectangle for this object
    Rectangle (*collision_rectangle)(const struct object_t *optional_instance);
};
typedef struct method_table_t method_table;

struct object_t
{
    const method_table *m;

    Vector2 position;
    Vector2 speed;
    Color normal_color;
    Color collide_color;
    float rotation;
    float rotation_speed;
    float radius;
    int id;
};
typedef struct object_t object;

extern object *objects[object_count];
extern char object_names[object_count][object_name_length];

extern void initialise_objects();
extern void draw_objects();
extern bool out_of_bounds(const object *me);
extern bool out_of_bounds_x(const object *me);
extern bool out_of_bounds_y(const object *me);
extern void update_object(object *me);

#endif
