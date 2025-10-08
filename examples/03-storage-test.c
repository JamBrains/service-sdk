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

void assert_threshold(uint64_t want_amount) {
    uint64_t got_amount = jb_service_info().threshold_balance;
    if (got_amount != want_amount) {
        fprintf(stderr, "Threshold balance mismatch: got %lu, want %lu\n", got_amount, want_amount);
        POLKAVM_TRAP();
    }
}

void assert_byte_in_storage(uint64_t want_amount) {
    uint64_t got_amount = jb_service_info().bytes_in_storage;
    if (got_amount != want_amount) {
        fprintf(stderr, "Bytes in storage mismatch: got %lu, want %lu\n", got_amount, want_amount);
        POLKAVM_TRAP();
    }
}

void assert_items_in_storage(uint64_t want_amount) {
    uint64_t got_amount = jb_service_info().items_in_storage;
    if (got_amount != want_amount) {
        fprintf(stderr, "Items in storage mismatch: got %lu, want %lu\n", got_amount, want_amount);
        POLKAVM_TRAP();
    }
}

void assert_bytes_and_items_in_storage(uint64_t bytes, uint64_t items) {
    assert_byte_in_storage(bytes);
    assert_items_in_storage(items);
}

void assert_ok(jb_result_t result) {
    if (result != JB_OK) {
        fprintf(stderr, "Failed to write to storage: %s\n", jb_result_name(result));
        POLKAVM_TRAP();
    }
}

void jb_hook_accumulate(jb_accumulate_arguments_t*) {
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());
    uint64_t bytes_in_storage = 0; // a_o
    uint64_t items_in_storage = 0; // n_o
    assert_threshold(100);

    // Writing one storage key incurs 10 + 1 per byte
    assert_ok(jb_storage_kv_writes("MyKey", "123456"));
    bytes_in_storage += 34 + 31 + 6;
    items_in_storage += 1;
    assert_threshold(100 + 10 * items_in_storage + bytes_in_storage);
    assert_bytes_and_items_in_storage(bytes_in_storage, items_in_storage);

    // Another one
    assert_ok(jb_storage_kv_writes("MyKey2", "123456789"));
    bytes_in_storage += 34 + 31 + 9;
    items_in_storage += 1;
    assert_threshold(100 + 10 * items_in_storage + bytes_in_storage);
    assert_bytes_and_items_in_storage(bytes_in_storage, items_in_storage);

    // Edit existing key (len 6 -> 9)
    assert_ok(jb_storage_kv_writes("MyKey", "123456789"));
    bytes_in_storage += 3;
    assert_threshold(100 + 10 * items_in_storage + bytes_in_storage);
    assert_bytes_and_items_in_storage(bytes_in_storage, items_in_storage);

    // Edit existing key (len 9 -> 6)
    assert_ok(jb_storage_kv_writes("MyKey2", "123456"));
    bytes_in_storage -= 3;
    assert_threshold(100 + 10 * items_in_storage + bytes_in_storage);
    assert_bytes_and_items_in_storage(bytes_in_storage, items_in_storage);

    return;

    for (int i = 0; i < 1000; i++) {
        char key[100];
        memset(key, 0, sizeof(key));
        snprintf(key, sizeof(key), "MyKey#%d", i);
        jb_result_t result = jb_storage_kv_writes(key, "MyValue");

        if (result != JB_OK) {
            fprintf(stderr, "Failed to write to storage: %s\n", jb_result_name(result));
            break;
        }

        // Print our balance and storage deposit
        uint64_t balance = jb_service_balance();
        uint64_t storage_deposit = jb_service_info().threshold_balance;
        float used_percent = ((float)storage_deposit / (float)balance) * 100.f;
        printf("Balance: %lu, storage deposit: %lu, used: %.2f%%\n", balance, storage_deposit, used_percent);
    }

    printf("Finished. Remaining gas: %lu\n", jb_service_gas_remaining());
}
