#include "jb_service.h"
#include "jb_codec_derive.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test struct with simple fields.
typedef struct {
    uint8_t smaller;
    uint16_t small;
    uint32_t normal;
    uint64_t big;
    bool boolean;
} simple_data_t;

// Use AUTO_DECLARE_DECODER for automatic type detection and decoder generation.
// TODO: Make it work on array fields as well.
AUTO_DECLARE_DECODER(simple_data_t, smaller, small, normal, big, boolean);

// Test function to verify decoding works
static void test_codec_derive() {
    puts("Testing codec derive macros...");
    
    // Test data for simple struct
    uint8_t simple_data[] = {
        0x2A, // smaller: 42
        0x39, 0x05, // small: 1337 (little endian)
        0xD9, 0x6D, 0x06, 0x00, // normal: 421337 (little endian)
        0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // big: 4294967297 (little endian)
        0x01 // boolean: true
    };
    
    uint8_t* buffer = simple_data;
    uint64_t remaining = sizeof(simple_data);
    simple_data_t decoded_simple;
    
    jb_result_t result = decode_simple_data_t(&buffer, &remaining, &decoded_simple);
    if (result == JB_OK) {
        puts("Simple struct decoded successfully");
    } else {
        fprintf(stderr, "Simple struct decode failed");
    }

    printf(
        "Decoded value: smaller=%u, small=%u, normal=%u, big=%lu, boolean=%u\n",
        decoded_simple.smaller,
        decoded_simple.small,
        decoded_simple.normal,
        decoded_simple.big,
        decoded_simple.boolean
    );
}

void jb_hook_accumulate(jb_accumulate_arguments_t* args) {
    puts("Multi-field codec test service starting");
    
    test_codec_derive();
}

void jb_hook_refine(jb_refine_arguments_t* args) {}
