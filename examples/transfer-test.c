#include "host_functions.h"
#include "jb_service.h"

#include <stdio.h>

#define MEMO_MAX_SIZE 128

void jb_hook_accumulate(jb_accumulate_arguments_t *args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_operands);

    puts("=== Transfer Host Function Test ===");
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // Let's send a message to a hardcoded service.
    uint64_t receiver_id = 2000000;
    uint64_t amount = 10000000000;
    uint64_t gas_limit = 100000;

    char memo[MEMO_MAX_SIZE] = "Hello";

    uint64_t result = jb_host_transfer(receiver_id, amount, gas_limit, (uint8_t const*)memo);
    if (result != HOST_OK) {
        fprintf(stderr, "Transfer failed: %s\n", jb_host_result_name(result));
        POLKAVM_TRAP();
    }

    puts("Transfer sent successfully!");
    printf("Remaining gas: %lu\n", jb_service_gas_remaining());
}

void jb_hook_is_authorized() {}
void jb_hook_refine() {}
