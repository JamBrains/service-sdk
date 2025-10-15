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

jb_result_t jb_refine_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_refine_arguments_t* out) {
	if (!out)
		return JB_ERR_NO_OUT;
	
	uint8_t* in_out_buff = in_buff;
	uint64_t in_out_remaining = in_remaining;
	jb_result_t res;
	
	// Decode core_index (uint32 as general_int)
	uint64_t core_index_u64;
	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &core_index_u64);
	if (res != JB_OK)
		return res;
	if (core_index_u64 > 0xFFFFFFFF)
		return JB_ERR_TOO_BIG;
	out->core_index = (uint32_t)core_index_u64;
	
	// Decode work_item_index (uint32 as general_int)
	uint64_t work_item_index_u64;
	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &work_item_index_u64);
	if (res != JB_OK)
		return res;
	if (work_item_index_u64 > 0xFFFFFFFF)
		return JB_ERR_TOO_BIG;
	out->work_item_index = (uint32_t)work_item_index_u64;
	
	// Decode service_id (uint32 as general_int)
	uint64_t service_id_u64;
	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &service_id_u64);
	if (res != JB_OK)
		return res;
	if (service_id_u64 > 0xFFFFFFFF)
		return JB_ERR_TOO_BIG;
	out->service_id = (uint32_t)service_id_u64;
	
	// Decode work_payload (binary)
	res = jb_codec_decode_binary(&in_out_buff, &in_out_remaining, &out->work_payload, &out->work_payload_len);
	if (res != JB_OK)
		return res;
	
	// Decode work_package_hash (h256 - fixed 32 bytes)
	res = jb_codec_decode_fixed_binary(&in_out_buff, &in_out_remaining, out->work_package_hash, 32);
	if (res != JB_OK)
		return res;
	
	if (in_out_remaining > 0)
		return JB_ERR_TRAILING_DATA;
	
	return JB_OK;
}

uint64_t jb_refine_arguments_fmt(jb_refine_arguments_t* args, char* buffer, uint64_t buffer_len) {
	return snprintf(buffer, buffer_len, 
		"core_index: %u\nwork_item_index: %u\nservice_id: %u\nwork_payload_len: %lu", 
		args->core_index, args->work_item_index, args->service_id, args->work_payload_len);
}
