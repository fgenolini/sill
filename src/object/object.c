#include <stddef.h>
#include <stdio.h>

#include "init.h"
#include "object/dice.h"
#include "object/egg.h"
#include "object/object.h"

static object dice1 = {0};
static object egg1 = {0};
static object egg2 = {0};

object *objects[object_count] = {&dice1, &egg1, &egg2};

void initialise_objects()
{
    dice1.class_type = &dice;
    egg1.class_type = &egg;
    egg2.class_type = &egg;
    for (int i = 0; i < object_count; ++i)
        objects[i]->class_type->init(i, objects[i]);

    sprintf(dice1.name, "dice_000001");
    sprintf(egg1.name, "egg_000001");
    sprintf(egg2.name, "egg_000002");
    egg2.position = (Vector2){egg1.position.x + 130.0f,
                              egg1.position.y + 40.0f};
}

void draw_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->draw(objects[i]);
}
