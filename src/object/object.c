#include <stddef.h>
#include <stdio.h>

#include "init.h"
#include "object/dice.h"
#include "object/egg.h"
#include "object/object.h"
#include "say.h"

static object dice1 = {0};
static object egg1 = {0};
static object egg2 = {0};

obj objects[object_count] = {{&dice, &dice1}, {&egg, &egg1}, {&egg, &egg2}};

static char buffer[256];

void initialise_objects()
{
    for (int i = 0; i < object_count; ++i)
    {
        objects[i].class_type->init(i, objects[i].instance);
    }

    sprintf(egg2.name, "egg2");
    egg2.position = (Vector2){egg1.position.x + 130.0f,
                              egg1.position.y + 40.0f};
}

void draw_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i].class_type->draw(objects[i].instance);
}
