#include "jb_service.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define VALIDATOR_SIZE 336
// Only aiming at the tiny configuration.
#define NUMBER_OF_VALIDATORS 6

void jb_hook_accumulate(jb_accumulate_arguments_t *args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_inputs);

    puts("=== Designate Host Function Test ===");
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // Designate only takes an array of new validator keys.
    uint8_t validator_keys[NUMBER_OF_VALIDATORS * VALIDATOR_SIZE];

    for (int i = 0; i < NUMBER_OF_VALIDATORS; i++) {
        for (int j = 0; j < VALIDATOR_SIZE; j++) {
            validator_keys[i * VALIDATOR_SIZE + j] = (i + j) % 256;
        }
    }

    puts("Calling designate host function with:");
    printf("  validators: 6 keys (336 bytes each)\n");

    uint64_t result = jb_host_designate(validator_keys);
    if (result != HOST_OK) {
        fprintf(stderr, "Call to designate failed: %s\n", jb_host_result_name(result));
        POLKAVM_TRAP();
    }

    puts("Designate operation completed successfully!");
    printf("Remaining gas: %lu\n", jb_service_gas_remaining());
}

void jb_hook_is_authorized() { /* Not needed */ }
void jb_hook_refine() { /* Not needed */ }
