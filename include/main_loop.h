#ifndef __MAIN_LOOP_H__
#define __MAIN_LOOP_H__

struct object_t
{
    Vector2 position;
    Vector2 speed;
    Color normal_color;
    Color collide_color;
    float rotation;
    float rotation_speed;
};

typedef struct object_t object;

extern bool has_sill_texture;
extern int sill_texture_x;
extern int sill_texture_y;

extern void main_loop();

#endif
