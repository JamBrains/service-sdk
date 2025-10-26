#include "host_functions.h"
#include "jb_codec.h"
#include "jb_codec_derive.h"
#include "jb_service.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// === SUMMATION ===
//
// A service that adds numbers.
//
// This service's refine takes in a list of numbers as the payload and returns their sum.
//
// Accumulate then stores that sum under the key "sum".

// The payload expected by this service's refine.
typedef struct {
    uint64_t count;
    uint64_t *numbers;
} refine_payload_t;

// The authorizer trace.
typedef struct {
    uint8_t result;
} authorizer_trace_t;

// Auto-generate decoder.
JB_GENERATE_DECODER(authorizer_trace_t, JB_CODEC_FIELD(u8, result))

// Need to manually decode the payload since AUTO_DECLARE_DECODER can't handle dynamic arrays yet.
jb_result_t decode_payload(uint8_t **payload, uint64_t length, refine_payload_t *out) {
    uint8_t *buffer = *payload;
    uint64_t remaining = length;

    uint64_t count;
    assert_ok(jb_codec_decode_general_int(&buffer, &remaining, &count));
    out->count = count;

    uint64_t *numbers = malloc(sizeof(uint64_t) * count);
    for (int i = 0; i < count; i++) {
        uint64_t number;
        assert_ok(jb_codec_decode_u64(&buffer, &remaining, &number));
        numbers[i] = number;
    }
    out->numbers = numbers;

    return JB_OK;
}

void jb_hook_refine(jb_refine_arguments_t *args) {
    printf("Refine called with core_index: %u, work_item_index: %u, service_id: %u, work_payload_len: %lu\n", args->core_index, args->work_item_index, args->service_id, args->work_payload_len);

    puts("=== REFINE ===");

    // We check out the authorizer trace.
    uint64_t encoded_len = jb_host_fetch(NULL, 0, 0, JB_FETCH_DISCRIMINATOR_AUTH_TRACE, 0, 0);
    if (encoded_len == 0) {
        fprintf(stderr, "Couldn't fetch.");
        POLKAVM_TRAP();
    }

    uint8_t *authorizer_trace_buff = (uint8_t*)malloc(encoded_len);
    if (!authorizer_trace_buff) {
        fprintf(stderr, "Couldn't request memory.");
        POLKAVM_TRAP();
    }

    uint8_t *original_trace_buff = authorizer_trace_buff;

    assert_host_ok(jb_host_fetch(authorizer_trace_buff, 0, encoded_len, JB_FETCH_DISCRIMINATOR_AUTH_TRACE, 0, 0));

    authorizer_trace_t authorizer_trace;
    assert_ok(jb_codec_decode_authorizer_trace_t(&authorizer_trace_buff, &encoded_len, &authorizer_trace));
    printf("Authorizer trace: %u\n", authorizer_trace.result);

    encoded_len = jb_host_fetch(NULL, 0, 0, JB_FETCH_DISCRIMINATOR_NTH_ITEM_PAYLOAD, args->work_item_index, 0);
    if (encoded_len == 0) {
        fprintf(stderr, "Couldn't fetch.");
        POLKAVM_TRAP();
    }

    uint8_t *buff = (uint8_t*)malloc(encoded_len);
    if (!buff) {
        fprintf(stderr, "Couldn't request memory.");
        POLKAVM_TRAP();
    }

    // Before it's modified by `jb_codec`.
    uint8_t *original_buff = buff;

    assert_host_ok(jb_host_fetch(buff, 0, encoded_len, JB_FETCH_DISCRIMINATOR_NTH_ITEM_PAYLOAD, args->work_item_index, 0));

    refine_payload_t payload;
    assert_ok(decode_payload(&buff, encoded_len, &payload));

    // We do the actual sum.
    uint64_t sum = 0;
    for (int i = 0; i < payload.count; i++) {
        if (i % 1000 == 0) {
            printf("Progress... %i\n", i);
        }
        sum += payload.numbers[i];
    }

    free(original_buff);
    free(payload.numbers);

    return_to_host((uint8_t*)&sum, sizeof(uint64_t));
}

void jb_hook_accumulate(jb_accumulate_arguments_t *args) {
    printf("Accumulate called with timeslot: %u, service_id: %u, num_operands: %lu\n", args->timeslot, args->service_id, args->num_inputs);

    puts("=== ACCUMULATE ===");

    uint64_t encoded_len = jb_host_fetch(NULL, 0, 0, JB_FETCH_DISCRIMINATOR_INPUTS, 0, 0);
    if (encoded_len == 0) {
        fprintf(stderr, "Couldn't fetch.");
        POLKAVM_TRAP();
    }

    uint8_t *buff = (uint8_t*)malloc(encoded_len);
    if (!buff) {
        fprintf(stderr, "Couldn't request memory.");
        POLKAVM_TRAP();
    }

    // We have to copy the original pointer since `jb_host_fetch` will modify `buff`.
    uint8_t *original_buff = buff;

    assert_host_ok(jb_host_fetch(buff, 0, encoded_len, JB_FETCH_DISCRIMINATOR_INPUTS, 0, 0));

    uint64_t inputs_len;
    uint64_t remaining = encoded_len;
    jb_result_t result = jb_codec_decode_general_int(&buff, &remaining, &inputs_len);
    if (result != JB_OK) {
        fprintf(stderr, "Couldn't decode number of inputs.");
        POLKAVM_TRAP();
    }

    jb_accumulate_input_t input;
    result = jb_accumulate_input_decode(&buff, &remaining, &input);
    if (result != JB_OK) {
        fprintf(stderr, "Couldn't decode accumulate input. result: %u", result);
        POLKAVM_TRAP();
    }

    printf("Input is a transfer? %u\n", input.is_transfer);
    printf("Gas limit: %lu\n", input.operand.gas_limit);

    uint8_t *buffer = input.operand.digest_output.data;
    uint64_t sum;
    remaining = sizeof(uint64_t);
    assert_ok(jb_codec_decode_u64(&buffer, &remaining, &sum));

    printf("Sum from refine: %lu\n", sum);

    char const *key = "sum";
    assert_ok(jb_storage_kv_write(
      (uint8_t*)key,
      strlen(key),
      (uint8_t*)&sum,
      sizeof(uint64_t),
      NULL
    ));

    // We free the allocated memory when we're done.
    free(original_buff);
}
