/**

THIS FILE IMPLEMENTS BEHAVIOUR OF THE JAM GRAY PAPER

*/

#pragma once

#include "jb_result.h"

#include <stdint.h>

jb_result_t jb_codec_decode_general_int(uint8_t** in_out_buff, uint64_t* in_out_remaining, uint64_t* out_value);
