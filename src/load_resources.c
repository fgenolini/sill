#include "raylib.h"

#include "init.h"
#include "load_resources.h"
#include "say.h"

loop_state main_state = {0};

static const char *sill_image = "resources/sill.png";

void load_resources()
{
    static const char *func = "load_resources";
    say(__FILE__, func, __LINE__, LOG_INFO, "loading sill.png");
    Image sill = LoadImage(sill_image);
    if (!sill.data || !sill.width || !sill.height)
    {
        say(__FILE__, func, __LINE__, LOG_ERROR,
            "could not load image sill.png");
        return;
    }

    main_state.sill_texture = LoadTextureFromImage(sill);
    UnloadImage(sill);
    if (!main_state.sill_texture.id)
    {
        say(__FILE__, func, __LINE__, LOG_ERROR,
            "could not load texture from sill.png");
        return;
    }

    main_state.has_sill_texture = true;
    main_state.sill_texture_x = (int)screen_width / 2 -
                                main_state.sill_texture.width / 2;
    main_state.sill_texture_y = (int)screen_height / 2 -
                                main_state.sill_texture.height / 2;
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}

void unload_resources()
{
    static const char *func = "unload_resources";
    if (!main_state.sill_texture.id)
        return;

    say(__FILE__, func, __LINE__, LOG_INFO, "unloading textures");
    UnloadTexture(main_state.sill_texture);
    main_state.sill_texture = (const Texture2D){0};
}
