// basic graphical object, such as a square or a circle

#ifndef __OBJECT_OBJECT_H__
#define __OBJECT_OBJECT_H__

#include "raylib.h"

struct object_t;
struct method_table_t
{
    /// @brief for classes only: initialise an object instance of that class
    void (*init)(int object_id, struct object_t *optional_instance);

    /// @brief draw the object at position with rotation
    void (*draw)(const struct object_t *me);

    /// @brief call update_object to set position, speed and rotation
    void (*update)(struct object_t *me);

    /// @brief estimate what the speed would be after a single collision
    Vector2 (*speed_after)(const struct object_t *me,
                           int collision_id, int other_end,
                           float *rotation_speed);

    /// @brief approximate the collision rectangle for this object
    Rectangle (*collision_rectangle)(const struct object_t *me);
};
typedef struct method_table_t method_table;

struct object_t
{
    const method_table *m;

    int id;
    Color collide_color;
    Color normal_color;
    Vector2 position;
    Vector2 speed;
    float radius;
    float rotation;
    float rotation_speed;
};
typedef struct object_t object;

extern void draw_objects();
extern object *get_object(int id);
extern int get_object_count();
extern void initialise_objects();
extern bool out_of_bounds(const object *me);
extern bool out_of_bounds_x(const object *me);
extern bool out_of_bounds_y(const object *me);
extern void set_object_name(int id, const char *name);
extern void update_object(object *me);

#endif
