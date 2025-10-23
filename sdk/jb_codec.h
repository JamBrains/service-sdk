/**

THIS FILE IMPLEMENTS BEHAVIOUR OF THE JAM GRAY PAPER

*/

#pragma once

#include "jb_result.h"

#include <stdint.h>
#include <stdbool.h>

// Basic type decoders
jb_result_t jb_codec_decode_general_int(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t* out_value);
jb_result_t jb_codec_decode_bool(uint8_t** in_out_buff, uint64_t* in_out_remaining, bool* out_value);
jb_result_t jb_codec_decode_u8(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint8_t* out_value);
jb_result_t jb_codec_decode_u16(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint16_t* out_value);
jb_result_t jb_codec_decode_u24(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint32_t* out_value);
jb_result_t jb_codec_decode_u32(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint32_t* out_value);
jb_result_t jb_codec_decode_u64(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t* out_value);

// Binary/string decoders
jb_result_t jb_codec_decode_binary(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint8_t** out_data, uint64_t* out_len);
jb_result_t jb_codec_decode_bounded_binary(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint8_t** out_data, uint64_t* out_len, uint64_t max_len);
jb_result_t jb_codec_decode_fixed_binary(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint8_t* out_data, uint64_t expected_len);

// Option/Maybe decoder
jb_result_t jb_codec_decode_maybe(uint8_t** in_out_buff, uint64_t* in_out_remaining, bool* out_has_value);

// List decoder (caller must handle individual item decoding)
jb_result_t jb_codec_decode_list_len(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t* out_len);

// Basic type encoders  
jb_result_t jb_codec_encode_general_int(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t value);
jb_result_t jb_codec_encode_bool(uint8_t** in_out_buff, uint64_t* in_out_remaining, bool value);
jb_result_t jb_codec_encode_u8(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint8_t value);
jb_result_t jb_codec_encode_u16(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint16_t value);
jb_result_t jb_codec_encode_u24(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint32_t value);
jb_result_t jb_codec_encode_u32(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint32_t value);
jb_result_t jb_codec_encode_u64(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t value);

// Binary/string encoders
jb_result_t jb_codec_encode_binary(uint8_t** in_out_buff, uint64_t* in_out_remaining, const uint8_t* data, uint64_t len);
jb_result_t jb_codec_encode_fixed_binary(uint8_t** in_out_buff, uint64_t* in_out_remaining, const uint8_t* data, uint64_t len);

// Option/Maybe encoder
jb_result_t jb_codec_encode_maybe_none(uint8_t** in_out_buff, uint64_t* in_out_remaining);
jb_result_t jb_codec_encode_maybe_some(uint8_t** in_out_buff, uint64_t* in_out_remaining);

// List encoder
jb_result_t jb_codec_encode_list_len(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t len);
