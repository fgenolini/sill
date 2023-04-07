#include "init.h"
#include "load_resources.h"
#include "main_loop.h"

int main()
{
    init();
    load_resources();
    main_loop();
}
