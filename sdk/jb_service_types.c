#include "jb_service_types.h"
#include "jb_codec.h"

#include <stdio.h>

jb_result_t jb_accumulate_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_accumulate_arguments_t* out) {
	uint64_t timeslot = 0, service_id = 0, num_operands = 0;

	// Convert our buffers since we dont expect trailing data.
	uint8_t* in_out_buff = in_buff;
	uint64_t in_out_remaining = in_remaining;

	jb_result_t res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &timeslot);
	if (res != JB_OK)
		return res;
	if (timeslot > 0xFFFFFFFF)
		return JB_ERR_TOO_BIG; // not an u32

	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &service_id);
	if (res != JB_OK)
		return res;
	if (service_id > 0xFFFFFFFF)
		return JB_ERR_TOO_BIG; // not an u32

	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &num_operands);
	if (res != JB_OK)
		return res;

	out->timeslot = (uint32_t)timeslot; // cast checked above
	out->service_id = (uint32_t)service_id; // cast checked above
	out->num_operands = num_operands;

	if (in_out_remaining > 0)
		return JB_ERR_TRAILING_DATA;

	return JB_OK;
}

uint64_t jb_accumulate_arguments_fmt(jb_accumulate_arguments_t* args, char* buffer, uint64_t buffer_len) {
	return snprintf(buffer, buffer_len, "timeslot: %u\nservice_id: %u\nnum_operands: %lu", args->timeslot, args->service_id, args->num_operands);
}
