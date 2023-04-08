#include "object/dice.h"
#include "object/egg.h"
#include "object/object.h"

object *objects[object_count] = {&dice, &egg};

void initialise_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->init(i);
}

void draw_objects()
{
    for (int i = 0; i < object_count; ++i)
        objects[i]->draw();
}
