#include "jb_service.h"
#include "jb_codec_derive.h"
#include "jb_assert.h"

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

JB_GENERATE_DECODER(simple_data_t,
    JB_CODEC_FIELD(u8, smaller)
    JB_CODEC_FIELD(u16, small)
    JB_CODEC_FIELD(u32, normal)
    JB_CODEC_FIELD(u64, big)
    JB_CODEC_FIELD(bool, boolean)
)

// Struct that contains the simple struct and some other fields.
typedef struct {
    uint16_t data;
    simple_data_t simple;
    uint8_t tiny;
} nested_data_t;

JB_GENERATE_DECODER(nested_data_t,
    JB_CODEC_FIELD(u16, data)
    JB_CODEC_FIELD(simple_data_t, simple)
    JB_CODEC_FIELD(u8, tiny)
)

typedef struct {
    uint8_t *data;
    uint64_t data_len;
} list_data_t;

JB_GENERATE_DECODER(list_data_t,
    JB_CODEC_FIELD_LIST(u8, data)
)

// struct with a list of lists
typedef struct {
    list_data_t *lists;
    uint64_t lists_len;
} list_of_lists_data_t;

JB_GENERATE_DECODER(list_of_lists_data_t,
    JB_CODEC_FIELD_LIST(list_data_t, lists)
)

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
    
    jb_result_t result = jb_codec_decode_simple_data_t(&buffer, &remaining, &decoded_simple);
    jb_assert_ok(result, "Failed to decode simple data");

    jb_assert_equal(decoded_simple.smaller, 42, "smaller value mismatch");
    jb_assert_equal(decoded_simple.small, 1337, "small value mismatch");
    jb_assert_equal(decoded_simple.normal, 421337, "normal value mismatch");
    jb_assert_equal(decoded_simple.big, 4294967297, "big value mismatch");
    jb_assert_equal(decoded_simple.boolean, true, "boolean value mismatch");

    uint8_t nested_data[] = {
        0x38, 0x05, // data: 1336 
        // small struct
        0x2A, // smaller: 42
        0x39, 0x05, // small: 1337 (little endian)
        0xD9, 0x6D, 0x06, 0x00, // normal: 421337 (little endian)
        0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // big: 4294967297 (little endian)
        0x01, // boolean: true
        // end of small struct
        45, // tiny: 45
    };

    buffer = nested_data;
    remaining = sizeof(nested_data);
    nested_data_t decoded_nested;
    result = jb_codec_decode_nested_data_t(&buffer, &remaining, &decoded_nested);
    jb_assert_ok(result, "Failed to decode nested data");

    jb_assert_equal(decoded_nested.data, 1336, "data value mismatch");
    jb_assert_equal(decoded_nested.simple.smaller, 42, "smaller value mismatch");
    jb_assert_equal(decoded_nested.simple.small, 1337, "small value mismatch");
    jb_assert_equal(decoded_nested.simple.normal, 421337, "normal value mismatch");
    jb_assert_equal(decoded_nested.simple.big, 4294967297, "big value mismatch");
    jb_assert_equal(decoded_nested.simple.boolean, true, "boolean value mismatch");
    jb_assert_equal(decoded_nested.tiny, 45, "tiny value mismatch");

    uint8_t list_data[] = {
        0x03, // list length: 3
        0x01, 0x02, 0x03, // list data: 1, 2, 3
    };

    buffer = list_data;
    remaining = sizeof(list_data);
    list_data_t decoded_list;
    result = jb_codec_decode_list_data_t(&buffer, &remaining, &decoded_list);
    jb_assert_ok(result, "Failed to decode list data");

    jb_assert_equal(decoded_list.data_len, 3, "list data length mismatch");
    jb_assert_equal(decoded_list.data[0], 1, "list data value mismatch");
    jb_assert_equal(decoded_list.data[1], 2, "list data value mismatch");
    jb_assert_equal(decoded_list.data[2], 3, "list data value mismatch");

    uint8_t list_of_lists_data[] = {
        0x02, // list length: 2
        0x03, // list length: 3
        0x01, 0x02, 0x03, // list data: 1, 2, 3
        0x03, // list length: 3
        0x04, 0x05, 0x06, // list data: 4, 5, 6
    };

    buffer = list_of_lists_data;
    remaining = sizeof(list_of_lists_data);
    list_of_lists_data_t decoded_list_of_lists;
    result = jb_codec_decode_list_of_lists_data_t(&buffer, &remaining, &decoded_list_of_lists);
    jb_assert_ok(result, "Failed to decode list of lists data");

    jb_assert_equal(decoded_list_of_lists.lists_len, 2, "list of lists length mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[0].data[0], 1, "list data value mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[0].data[1], 2, "list data value mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[0].data[2], 3, "list data value mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[1].data_len, 3, "list data length mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[1].data[0], 4, "list data value mismatch");
    jb_assert_equal(decoded_list_of_lists.lists[1].data[1], 5, "list data value mismatch");
}

void jb_hook_accumulate(jb_accumulate_arguments_t* args) {
    puts("Multi-field codec test service starting");
    
    test_codec_derive();
}

void jb_hook_refine(jb_refine_arguments_t* args) {}
