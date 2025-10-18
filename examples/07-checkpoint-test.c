#include "jb_service.h"
#include "jb_storage.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void jb_hook_accumulate(jb_accumulate_arguments_t *args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_inputs);
    
    puts("=== Designate Host Function Test ===");
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // We first change some state.
    assert_ok(jb_storage_kv_writes("TestKey", "Before checkpoint"));

    puts("Wrote to storage");

    // Checkpoint doesn't take any parameters.
    uint64_t gas_after_call = jb_host_checkpoint();

    // We change it again after the checkpoint.
    assert_ok(jb_storage_kv_writes("TestKey", "After checkpoint"));

    puts("Now we fail and rollback");

    // We trap, which rolls everything back until the latest checkpoint.
    POLKAVM_TRAP();
}

void jb_hook_is_authorized() {}
void jb_hook_refine() {}
