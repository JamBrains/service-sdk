/**
 * Automatic codec derivation macros for JAM Service SDK
 * 
 * Usage:
 *   DECLARE_DECODER(struct_name, field1_type, field1_name, field2_type, field2_name, ...)
 *   
 * For arrays: DECLARE_DECODER_WITH_ARRAY(struct_name, count_field, array_field, element_type)
 */

#pragma once

#include "jb_codec.h"
#include "jb_result.h"

// Simple macro for single field types
#define DECODE_u8(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_u8(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

#define DECODE_u16(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_u16(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

#define DECODE_u32(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_u32(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

#define DECODE_u64(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_u64(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

#define DECODE_bool(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_bool(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

#define DECODE_general_int(buffer, remaining, field) \
    do { \
        jb_result_t result = jb_codec_decode_general_int(buffer, remaining, field); \
        if (result != JB_OK) return result; \
    } while(0)

// Macro for dynamic arrays
#define DECODE_ARRAY_u64(buffer, remaining, array_field, count) \
    do { \
        array_field = malloc(sizeof(uint64_t) * count); \
        if (!array_field) return JB_ERR_MALLOC; \
        for (uint64_t i = 0; i < count; i++) { \
            jb_result_t result = jb_codec_decode_u64(buffer, remaining, &array_field[i]); \
            if (result != JB_OK) { \
                free(array_field); \
                return result; \
            } \
        } \
    } while(0)

// Specific decoders for common patterns
#define DECLARE_DECODER_1_FIELD(struct_name, type1, field1) \
    jb_result_t decode_##struct_name(uint8_t** buffer, uint64_t* remaining, struct_name* out) { \
        DECODE_##type1(buffer, remaining, &out->field1); \
        return JB_OK; \
    }

#define DECLARE_DECODER_2_FIELDS(struct_name, type1, field1, type2, field2) \
    jb_result_t decode_##struct_name(uint8_t** buffer, uint64_t* remaining, struct_name* out) { \
        DECODE_##type1(buffer, remaining, &out->field1); \
        DECODE_##type2(buffer, remaining, &out->field2); \
        return JB_OK; \
    }

// Special decoder for struct with count + array
#define DECLARE_DECODER_WITH_ARRAY(struct_name, count_field, array_field) \
    jb_result_t decode_##struct_name(uint8_t** buffer, uint64_t* remaining, struct_name* out) { \
        DECODE_general_int(buffer, remaining, &out->count_field); \
        DECODE_ARRAY_u64(buffer, remaining, out->array_field, out->count_field); \
        return JB_OK; \
    }