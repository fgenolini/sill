#ifndef __SAY_H__
#define __SAY_H__

#include "raylib.h"

typedef TraceLogLevel log_level;

// Say something in the console output
extern void say(const char *file, const char *func, int line,
                log_level level, const char *text);

#endif
