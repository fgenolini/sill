#include "raylib.h"
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

    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}
