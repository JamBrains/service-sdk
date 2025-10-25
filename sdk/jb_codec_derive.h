/**
 * Automatic codec derivation macros for JAM Service SDK
 * 
 * Usage:
 *   AUTO_DECLARE_DECODER(struct_name, field1_type, field1_name, field2_type, field2_name, ...)
 *
 * TODO: Currently can't handle dynamic arrays or nested custom types.
 */

#pragma once

#include "jb_codec.h"
#include "jb_result.h"

#include <stdbool.h>

// Macro to automatically decode a field by detecting its type
#define AUTO_DECODE_FIELD(struct_type, field_name) \
    do { \
        jb_result_t result = _Generic((((struct_type*)0)->field_name), \
            uint8_t: jb_codec_decode_u8, \
            uint16_t: jb_codec_decode_u16, \
            uint32_t: jb_codec_decode_u32, \
            uint64_t: jb_codec_decode_u64, \
            bool: jb_codec_decode_bool, \
            int: jb_codec_decode_u32, \
            default: jb_codec_decode_u64)(buffer, remaining, &out->field_name); \
        if (result != JB_OK) return result; \
    } while(0);

// Fully automatic decoder that derives everything from struct definition
#define AUTO_DECLARE_DECODER_1(struct_type, f1) \
    jb_result_t decode_##struct_type(uint8_t** buffer, uint64_t* remaining, struct_type* out) { \
        AUTO_DECODE_FIELD(struct_type, f1) \
        return JB_OK; \
    }

#define AUTO_DECLARE_DECODER_2(struct_type, f1, f2) \
    jb_result_t decode_##struct_type(uint8_t** buffer, uint64_t* remaining, struct_type* out) { \
        AUTO_DECODE_FIELD(struct_type, f1) \
        AUTO_DECODE_FIELD(struct_type, f2) \
        return JB_OK; \
    }

#define AUTO_DECLARE_DECODER_3(struct_type, f1, f2, f3) \
    jb_result_t decode_##struct_type(uint8_t** buffer, uint64_t* remaining, struct_type* out) { \
        AUTO_DECODE_FIELD(struct_type, f1) \
        AUTO_DECODE_FIELD(struct_type, f2) \
        AUTO_DECODE_FIELD(struct_type, f3) \
        return JB_OK; \
    }

#define AUTO_DECLARE_DECODER_4(struct_type, f1, f2, f3, f4) \
    jb_result_t decode_##struct_type(uint8_t** buffer, uint64_t* remaining, struct_type* out) { \
        AUTO_DECODE_FIELD(struct_type, f1) \
        AUTO_DECODE_FIELD(struct_type, f2) \
        AUTO_DECODE_FIELD(struct_type, f3) \
        AUTO_DECODE_FIELD(struct_type, f4) \
        return JB_OK; \
    }

#define AUTO_DECLARE_DECODER_5(struct_type, f1, f2, f3, f4, f5) \
    jb_result_t decode_##struct_type(uint8_t** buffer, uint64_t* remaining, struct_type* out) { \
        AUTO_DECODE_FIELD(struct_type, f1) \
        AUTO_DECODE_FIELD(struct_type, f2) \
        AUTO_DECODE_FIELD(struct_type, f3) \
        AUTO_DECODE_FIELD(struct_type, f4) \
        AUTO_DECODE_FIELD(struct_type, f5) \
        return JB_OK; \
    }

// Overloaded macro for automatic decoder generation
#define GET_AUTO_DECODER_MACRO(_1, _2, _3, _4, _5, _6, NAME, ...) NAME

#define AUTO_DECLARE_DECODER(...) GET_AUTO_DECODER_MACRO(__VA_ARGS__, \
    AUTO_DECLARE_DECODER_5, AUTO_DECLARE_DECODER_4, AUTO_DECLARE_DECODER_3, \
    AUTO_DECLARE_DECODER_2, AUTO_DECLARE_DECODER_1)(__VA_ARGS__)
