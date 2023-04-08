#ifndef __LOAD_RESOURCES_H__
#define __LOAD_RESOURCES_H__

#include "raylib.h"

struct loop_state_t
{
    Texture2D sill_texture;
    float delta_time;
    int sill_texture_x;
    int sill_texture_y;
    bool has_sill_texture;
};
typedef struct loop_state_t loop_state;

extern loop_state main_state;

extern void load_resources();
extern void unload_resources();

#endif
