#include "jb_service.h"
#include "host_functions.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define NAME "Bless Demo"

void jb_hook_on_transfer(char **_out_ptr, uint64_t *_out_len) {
    // Not implemented for this demo
}

void jb_hook_is_authorized() {
    // Not implemented for this demo
}

void jb_hook_refine() {
    // Not implemented for this demo
}

void jb_hook_accumulate(jb_accumulate_arguments_t*) {
    printf("=== Bless Host Function Demo ===\n");
    printf("Balance: %lu, gas remaining: %lu\n", jb_service_balance(), jb_service_gas_remaining());

    // Example bless operation with sample parameters
    uint64_t manager = 12345;
    uint64_t delegator = 67890;
    uint64_t registrar = 11111;
    
    // Sample assigners data (could be account IDs or other identifiers)
    uint8_t assigners_data[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    
    // Extra data: accumulate services list
    // Each entry: 4 bytes service_id + 8 bytes gas
    struct accumulate_service {
        uint32_t service_id;
        uint64_t gas;
    } __attribute__((packed));
    
    struct accumulate_service services[] = {
        {.service_id = 1001, .gas = 50000},
        {.service_id = 1002, .gas = 75000},
        {.service_id = 1003, .gas = 100000}
    };
    
    uint64_t service_count = 3;

    printf("Calling bless host function with:\n");
    printf("  manager: %lu\n", manager);
    printf("  delegator: %lu\n", delegator);
    printf("  registrar: %lu\n", registrar);
    printf("  assigners_data: [");
    for (int i = 0; i < sizeof(assigners_data); i++) {
        printf("0x%02x", assigners_data[i]);
        if (i < sizeof(assigners_data) - 1) printf(", ");
    }
    printf("]\n");
    printf("  accumulate services (%lu entries):\n", service_count);
    for (int i = 0; i < service_count; i++) {
        printf("    service_id: %u, gas: %lu\n", services[i].service_id, services[i].gas);
    }

    // Call the bless host function
    jb_host_bless(manager, assigners_data, delegator, registrar, (uint8_t*)services, service_count);

    printf("Bless operation completed successfully!\n");
    printf("Remaining gas: %lu\n", jb_service_gas_remaining());
}
