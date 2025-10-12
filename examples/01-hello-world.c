#include "jb_storage.h"

#include <stdio.h>

void jb_hook_accumulate(jb_accumulate_arguments_t*) {
    puts("Hello World from Accumulate");
}

void jb_hook_refine() {
    puts("Hello World from Refine");
}

void jb_hook_is_authorized() {
    puts("Hello World from IsAuthorized");
}
