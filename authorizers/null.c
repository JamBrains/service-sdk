#include "jb_authorizer.h"

#include <stddef.h>
#include <stdio.h>

void jb_hook_is_authorized(jb_is_authorized_arguments_t* args) {
    printf("Calling is_authorized with core_index: %u and gas: %lu\n", args->core_index, jb_host_gas());

    // Always authorizes.
    // Doesn't return anything.
    uint8_t result = 1; // true
    return_to_host(&result, 1);
}
