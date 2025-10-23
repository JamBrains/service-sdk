#include "jb_service.h"
#include "jb_service_types.h"

#include <stdio.h>

void jb_hook_accumulate(jb_accumulate_arguments_t*) {
    puts("Hello World from Accumulate");
}

void jb_hook_refine(jb_refine_arguments_t*) {
    puts("Hello World from Refine");
}
