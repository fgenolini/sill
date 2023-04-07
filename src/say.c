#include <stdio.h>
#include <time.h>

#include "say.h"

static const char *levels[] = {
    "all",
    "trace",
    "debug",
    "info",
    "warning",
    "error",
    "fatal",
    "none"};

// Say something in the console output
void say(const char *file, const char *func, int line,
         log_level level, const char *text)
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    char buff[100];
    if (!strftime(buff, sizeof buff, "%FT%T", gmtime(&ts.tv_sec)))
    {
        buff[0] = 0;
    }

    printf("%s.%09ldZ,%s,%s,%d,%s,%s\n",
           buff, ts.tv_nsec, file, func, line, levels[level], text);
}
