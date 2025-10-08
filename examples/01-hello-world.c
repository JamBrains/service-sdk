#include "/opt/sdk/jb_service.h"
#include "jb_storage.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define NAME "JB Demo Service"

void jb_hook_on_transfer(char **_out_ptr, uint64_t *_out_len /* TODO: transfers arg */) {

}

void jb_hook_is_authorized(/* TODO: authorization args */) {

}

void jb_hook_refine(/* TODO: refine args */) {

}

void jb_hook_accumulate(jb_accumulate_arguments_t*) {
    puts("Hello from Accumulate");
}
