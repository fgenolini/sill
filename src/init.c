#include <locale.h>
#include <stdlib.h>

#include "raylib.h"

#include "load_resources.h"
#include "init.h"
#include "say.h"

enum init_internal_constant
{
    /// @brief frames per second
    fps = 30
};

static const char *title = "window sill - basic";
static const char *default_lang = "en_GB.utf8";

static void atexit_close_window()
{
    static const char *func = "atexit_close_window";
    say(__FILE__, func, __LINE__, LOG_INFO, "cleaning up");
    unload_resources();
    say(__FILE__, func, __LINE__, LOG_INFO, "closing window");
    CloseWindow();
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}

void init()
{
    static const char *func = "init";
    say(__FILE__, func, __LINE__, LOG_DEBUG, "hi");
    const char *lang = getenv("LANG");
    if (!lang)
        lang = default_lang;

    setlocale(LC_TIME, lang);
    say(__FILE__, func, __LINE__, LOG_INFO, lang);
    InitWindow(screen_width, screen_height, title);
    atexit(atexit_close_window);
    SetTargetFPS(fps);
    ToggleFullscreen();
    say(__FILE__, func, __LINE__, LOG_INFO, "done");
}
