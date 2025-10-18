#include "jb_service.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void jb_hook_accumulate(jb_accumulate_arguments_t *args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_inputs);

    puts("=== Assign Host Function Test ===");
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // This service needs to be called by the assigner of core 0 to have effect.
    uint64_t core_index = 0;
    
    // Create 80 authorizers (32 bytes each = 2560 bytes total)
    uint8_t authorizers[80 * 32];
    
    // Fill with sample hash data
    for (int i = 0; i < 80; i++) {
        for (int j = 0; j < 32; j++) {
            authorizers[i * 32 + j] = (i + j) % 256;
        }
    }

    // A new assigner for core 0 is set.
    uint64_t assigner = 12345;

    puts("Calling assign host function with:");
    printf("  core_index: %lu\n", core_index);
    printf("  authorizers: 80 hashes (32 bytes each)\n");
    printf("  assigner: %lu\n", assigner);
    
    // Call the assign host function
    uint64_t result = jb_host_assign(core_index, authorizers, assigner);
    if (result != HOST_OK) {
        fprintf(stderr, "Call to assign failed: %s\n", jb_host_result_name(result));
        POLKAVM_TRAP();
    }
    
    puts("Assign operation completed successfully!");
    printf("Remaining gas: %lu\n", jb_service_gas_remaining());
}

void jb_hook_is_authorized() { /* Not needed for this test */ }
void jb_hook_refine() {/* Not needed for this test */ }
