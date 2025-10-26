/**
 * Automatic codec derivation macros for JAM Service SDK
 * 
 * Usage:
 *   JB_GENERATE_DECODER(struct_name, JB_CODEC_FIELD(type, name), JB_CODEC_FIELD(type, name), ...)
 *
 */

#pragma once

#include "jb_codec.h"
#include "jb_result.h"

#include <stdbool.h>

#define JB_CODEC_FIELD(type, name) \
    do { \
        jb_result_t result = jb_codec_decode_##type(buffer, remaining, &out->name); \
        if (result != JB_OK) return result; \
    } while(0);

#define JB_CODEC_FIELD_LIST(type, name) \
    do { \
        jb_result_t result = jb_codec_decode_list_len(buffer, remaining, &out->name##_len); \
        if (result != JB_OK) return result; \
        if (out->name##_len == 0) { \
            out->name = NULL; \
            return JB_OK; \
        } \
        out->name = malloc(sizeof(typeof(out->name[0])) * out->name##_len); \
        if (out->name == NULL) return JB_ERR_MALLOC; \
        for (int i = 0; i < out->name##_len; i++) { \
            typeof(out->name[0]) value; \
            result = jb_codec_decode_##type(buffer, remaining, &value); \
            if (result != JB_OK) return result; \
            out->name[i] = value; \
        } \
    } while(0);

#define __JB_GENERATE_DECODER_3(NAME, PREFIX, ...) \
    jb_result_t PREFIX##NAME(uint8_t** buffer, uint64_t* remaining, NAME* out) { \
        __VA_ARGS__ \
        return JB_OK; \
    }

#define __JB_GENERATE_DECODER_2(NAME, ...) \
    __JB_GENERATE_DECODER_3(NAME, jb_codec_decode_, __VA_ARGS__)

#define __JB_GET_MACRO(_1, _2, _3, NAME, ...) NAME

/// @brief Generate a decoder for a given struct. Check `multi-field-test.c` for an example.
/// @param NAME The name of the struct to generate a decoder for.
/// @param ... The fields of the struct to decode. Must be in the form JB_CODEC_FIELD(type, name)
/// @details Available FIELD macros are: JB_CODEC_FIELD(type, name), JB_CODEC_FIELD_LIST(type, name)
/// @example JB_GENERATE_DECODER(rgb_color_t, JB_CODEC_FIELD(u8, r), JB_CODEC_FIELD(u8, g), JB_CODEC_FIELD(u8, b))
#define JB_GENERATE_DECODER(...) __JB_GET_MACRO(__VA_ARGS__, __JB_GENERATE_DECODER_3, __JB_GENERATE_DECODER_2)(__VA_ARGS__)
