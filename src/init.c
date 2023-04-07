#include <locale.h>
#include <stdlib.h>

#include "raylib.h"

#include "load_resources.h"
#include "say.h"

const float screen_width = 1920.0f;
const float screen_height = 1080.0f;

// Frames per second, usually 30 is enough,
//   as long as moving objects are not too fast
static const int fps = 30;

static const char *title = "window sill - basic";
static const char *default_lang = "en_GB.utf8";

static void atexit_close_window()
{
    const char *func = "atexit_close_window";
    say(__FILE__, func, __LINE__, LOG_INFO, "cleaning up");
    if (sill_texture.id)
    {
        say(__FILE__, func, __LINE__, LOG_INFO, "unloading textures");
        UnloadTexture(sill_texture);
        sill_texture = (const Texture2D){0};
    }

    say(__FILE__, func, __LINE__, LOG_INFO, "closing window");
    CloseWindow();
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}

void init()
{
    const char *func = "init";
    say(__FILE__, func, __LINE__, LOG_DEBUG, "hi");
    const char *lang = getenv("LANG");
    if (!lang)
        lang = default_lang;

    setlocale(LC_TIME, lang);
    say(__FILE__, func, __LINE__, LOG_INFO, lang);
    InitWindow((int)screen_width, (int)screen_height, title);
    atexit(atexit_close_window);
    SetTargetFPS(fps);
    ToggleFullscreen();
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}
