#include "jb_service.h"
#include "jb_storage.h"
#include "jb_codec.h"
#include "jb_service_types.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void jb_hook_accumulate(jb_accumulate_arguments_t* args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_inputs);
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // Print all chain params. Will produce something like this:
    //  item_deposit: 10,
    //  byte_deposit: 1,
    //  base_deposit: 100,
    //  core_count: 341,
    //  ...
    char buffer[1000];
    jb_chain_params_t params = jb_chain_params();
    jb_chain_params_fmt(&params, buffer, sizeof(buffer));
    printf("Chain Params: %s\n", buffer);

    // Print chain entropy
    char entropy[32];
    jb_chain_entropy_32((uint8_t*)entropy);
    printf("Entropy: 0x");
    for (int i = 0; i < 32; i++)
        printf("%02x", entropy[i]);
    puts("");

    // Write a value to storage
    jb_result_t res = jb_storage_kv_writes("MyKey", "MyValue");
    if (res != JB_OK) {
        fprintf(stderr, "Could not write key: %s", jb_result_name(res));
        return;
    }

    // Check if we can read the value we just wrote:
    uint8_t value[100];
    jb_result_t result = jb_storage_kv_reads("MyKey", value, 0, sizeof(value), NULL);

    if (result == JB_OK)
        printf("Value: %s\n", value);
    else
        fprintf(stderr, "Key is not present\n");

    // Try to write into the storage until our balance is insufficient:
    for (int i = 0; i < 1000; i++) {
        // We generate a new key `MyKey#<i>` for each item:
        char key[100];
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "MyKey#%d", i);
        jb_result_t result = jb_storage_kv_writes(key, "MyValue");

        if (result != JB_OK) {
            // We may not have enough gas remaining to print it, but still try:
            fprintf(stderr, "Writing to the storage failed: %s\n", jb_result_name(result));
            break;
        }

        // Print our balance and required threshold balance
        uint64_t balance = jb_service_balance();
        uint64_t storage_deposit = jb_service_info().threshold_balance;
        float used_percent = ((float)storage_deposit / (float)balance) * 100.f;
        printf("Items: %i, balance: %lu, storage deposit: %lu, used: %.2f%%\n", i + 1, balance, storage_deposit, used_percent);
    }

    printf("Finished. Remaining gas: %lu\n", jb_service_gas_remaining());
}


void jb_hook_is_authorized() {}
void jb_hook_refine() {}
