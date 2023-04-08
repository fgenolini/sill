#include "raylib.h"

#include "init.h"
#include "main_loop.h"
#include "say.h"

Texture2D sill_texture = {0};

static const char *sill_image = "resources/sill.png";

void load_resources()
{
    const char *func = "load_resources";
    say(__FILE__, func, __LINE__, LOG_INFO, "loading sill.png");
    Image sill = LoadImage(sill_image);
    if (!sill.data || !sill.width || !sill.height)
    {
        say(__FILE__, func, __LINE__, LOG_ERROR,
            "could not load image sill.png");
        return;
    }

    sill_texture = LoadTextureFromImage(sill);
    UnloadImage(sill);
    if (!sill_texture.id)
    {
        say(__FILE__, func, __LINE__, LOG_ERROR,
            "could not load texture from sill.png");
        return;
    }

    has_sill_texture = true;
    sill_texture_x = (int)screen_width / 2 - sill_texture.width / 2;
    sill_texture_y = (int)screen_height / 2 - sill_texture.height / 2;
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}
