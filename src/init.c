#include <locale.h>
#include <stdlib.h>

#include "raylib.h"
#include "say.h"

static const int screenWidth = 800;
static const int screenHeight = 450;

// Frames per second, usually 30 is enough
static const int fps = 30;

static const char *title = "window sill - basic";
static const char *default_lang = "en_GB.utf8";

static void atexit_close_window()
{
    CloseWindow();
    say(__FILE__, "atexit_close_window", __LINE__, "bye");
}

void init()
{
    const char *lang = getenv("LANG");
    if (!lang)
    {
        lang = default_lang;
    }

    setlocale(LC_TIME, lang);
    say(__FILE__, "init", __LINE__, "hi");
    say(__FILE__, "init", __LINE__, lang);
    InitWindow(screenWidth, screenHeight, title);
    atexit(atexit_close_window);
    SetTargetFPS(fps);
}
