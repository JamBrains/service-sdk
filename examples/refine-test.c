#include "jb_service.h"

#include <stdio.h>

void jb_hook_refine(jb_refine_arguments_t *args) {
    printf("Refine called with core_index: %u, work_item_index: %u, service_id: %u, work_payload_len: %lu\n", args->core_index, args->work_item_index, args->service_id, args->work_payload_len);

    puts("=== Refine Test ===");

    char *string = "Hello";
    uint8_t *bytes = (uint8_t*)string;

    uint64_t length = 5;

    return_to_host(bytes, length);
}

void jb_hook_accumulate(jb_accumulate_arguments_t *) {}
void jb_hook_is_authorized() {}
