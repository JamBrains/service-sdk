#include "jb_service_types.h"
#include "jb_codec.h"
#include "jb_result.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

jb_result_t jb_accumulate_arguments_decode(uint8_t* in_buff, uint64_t in_remaining, jb_accumulate_arguments_t* out) {
	uint64_t timeslot = 0, service_id = 0, num_inputs = 0;

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

	res = jb_codec_decode_general_int(&in_out_buff, &in_out_remaining, &num_inputs);
	if (res != JB_OK)
		return res;

	out->timeslot = (uint32_t)timeslot; // cast checked above
	out->service_id = (uint32_t)service_id; // cast checked above
	out->num_inputs = num_inputs;

	if (in_out_remaining > 0)
		return JB_ERR_TRAILING_DATA;

	return JB_OK;
}

uint64_t jb_accumulate_arguments_fmt(jb_accumulate_arguments_t* args, char* buffer, uint64_t buffer_len) {
	return snprintf(buffer, buffer_len, "timeslot: %u\nservice_id: %u\nnum_operands: %lu", args->timeslot, args->service_id, args->num_inputs);
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
	
	// Decode work_package_hash (h256 - fixed `HASH_SIZE` bytes)
	res = jb_codec_decode_fixed_binary(&in_out_buff, &in_out_remaining, out->work_package_hash, HASH_SIZE);
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

jb_result_t jb_work_digest_output_decode(uint8_t **in_buff, uint64_t *in_remaining, jb_work_digest_output_t *out) {
	if (!out) {
		return JB_ERR_NO_OUT;
	}

	uint8_t *in_out_buff = *in_buff;
	uint64_t *in_out_remaining = in_remaining;
	jb_result_t result;

	// First we decode the status - whether it succeeded or not.
	jb_work_digest_output_status_t status;
	result = jb_codec_decode_u8(&in_out_buff, in_out_remaining, &status);
	if (result != JB_OK) {
		return result;
	}
	out->status = status;

	if (status != DIGEST_OK) {
		// If there was an error, there's no data.
		out->data = NULL;
		out->data_len = 0;
		return JB_OK;
	}

	// Now we go for the data.
	uint64_t data_len;
	uint8_t *data;
	result = jb_codec_decode_binary(&in_out_buff, in_out_remaining, &data, &data_len);
	if (result != JB_OK) {
		return result;
	}
	out->data = data;
	out->data_len = data_len;

	// All good.
	return JB_OK;
}

jb_result_t jb_accumulate_operand_decode(uint8_t **in_buff, uint64_t *in_remaining, jb_accumulate_operand_t *out) {
	if (!out) {
		return JB_ERR_NO_OUT;
	}

	uint8_t *in_out_buff = *in_buff;
	uint64_t *in_out_remaining = in_remaining;
	jb_result_t result;

	uint8_t work_package_hash[HASH_SIZE];
	result = jb_codec_decode_fixed_binary(&in_out_buff, in_out_remaining, work_package_hash, HASH_SIZE);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode work_package_hash, result: %u\n", result);
		return result;
	}
	for (int i = 0; i < HASH_SIZE; i++) {
		out->work_package_hash[i] = work_package_hash[i];
	}

	uint8_t work_package_segment_root[HASH_SIZE];
	result = jb_codec_decode_fixed_binary(&in_out_buff, in_out_remaining, work_package_segment_root, HASH_SIZE);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode work_package_segment_root, result: %u\n", result);
		return result;
	}
	for (int i = 0; i < HASH_SIZE; i++) {
		out->work_package_segment_root[i] = work_package_segment_root[i];
	}

	uint8_t work_report_authorizer_hash[HASH_SIZE];
	result = jb_codec_decode_fixed_binary(&in_out_buff, in_out_remaining, work_report_authorizer_hash, HASH_SIZE);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode work_report_authorizer_hash, result: %u\n", result);
		return result;
	}
	for (int i = 0; i < HASH_SIZE; i++) {
		out->work_report_authorizer_hash[i] = work_report_authorizer_hash[i];
	}

	uint8_t work_digest_payload_hash[HASH_SIZE];
	result = jb_codec_decode_fixed_binary(&in_out_buff, in_out_remaining, work_digest_payload_hash, HASH_SIZE);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode work_digest_payload_hash, result: %u\n", result);
		return result;
	}
	for (int i = 0; i < HASH_SIZE; i++) {
		out->work_digest_payload_hash[i] = work_digest_payload_hash[i];
	}

	uint64_t gas_limit;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &gas_limit);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode gas_limit, result: %u\n", result);
		return result;
	}
	out->gas_limit = gas_limit;

	jb_work_digest_output_t digest_output;
	result = jb_work_digest_output_decode(&in_out_buff, in_out_remaining, &digest_output);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode digest_output, result: %u\n", result);
		return result;
	}
	out->digest_output = digest_output;

	uint64_t work_report_output_len;
	uint8_t *work_report_output;
	result = jb_codec_decode_binary(&in_out_buff, in_remaining, &work_report_output, &work_report_output_len);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode work_report_output, result: %u\n", result);
		return result;
	}
	out->work_report_output_len = work_report_output_len;
	out->work_report_output = work_report_output;

	return JB_OK;
}

jb_result_t jb_accumulate_transfer_decode(uint8_t **in_buff, uint64_t *in_remaining, jb_accumulate_transfer_t *out) {
	if (!out) {
		return JB_ERR_NO_OUT;
	}

	uint8_t *in_out_buff = *in_buff;
	uint64_t *in_out_remaining = in_remaining;
	jb_result_t result;

	uint64_t sender;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &sender);
	if (result != JB_OK) {
		return result;
	}
	if (sender > 0xFFFFFFFF) {
		return JB_ERR_TOO_BIG;
	}
	out->sender = (uint32_t)sender;

	uint64_t receiver;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &receiver);
	if (result != JB_OK) {
		return result;
	}
	if (receiver > 0xFFFFFFFF) {
		return JB_ERR_TOO_BIG;
	}
	out->receiver = (uint32_t)receiver;

	uint64_t amount;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &amount);
	if (result != JB_OK) {
		return result;
	}
	out->amount = amount;

	uint8_t memo[MAX_MEMO_SIZE];
	result = jb_codec_decode_fixed_binary(&in_out_buff, in_out_remaining, memo, MAX_MEMO_SIZE);
	if (result != JB_OK) {
		return result;
	}
	for (int i = 0; i < MAX_MEMO_SIZE; i++) {
		out->memo[i] = memo[i];
	}

	uint64_t gas_limit;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &gas_limit);
	if (result != JB_OK) {
		return result;
	}
	out->gas_limit = gas_limit;

	return JB_OK;
}

jb_result_t jb_accumulate_input_decode(uint8_t **in_buff, uint64_t *in_remaining, jb_accumulate_input_t *out) {
	if (!out) {
		return JB_ERR_NO_OUT;
	}

	uint8_t *in_out_buff = *in_buff;
	uint64_t *in_out_remaining = in_remaining;
	jb_result_t result;

	bool is_transfer;
	result = jb_codec_decode_bool(&in_out_buff, in_out_remaining, &is_transfer);
	if (result != JB_OK) {
		fprintf(stderr, "Couldn't decode the bool at the start, result: %u\n", result);
		return result;
	}
	out->is_transfer = is_transfer;

	if (is_transfer) {
		jb_accumulate_transfer_t transfer;
		result = jb_accumulate_transfer_decode(&in_out_buff, in_out_remaining, &transfer);
		if (result != JB_OK) {
			fprintf(stderr, "Couldn't decode the transfer inside the input, result: %u\n", result);
			return result;
		}
		out->transfer = transfer;
	} else {
		jb_accumulate_operand_t operand;
		result = jb_accumulate_operand_decode(&in_out_buff, in_out_remaining, &operand);
		if (result != JB_OK) {
			fprintf(stderr, "Couldn't decode the operand inside the input, result: %u\n", result);
			return result;
		}
		out->operand = operand;
	}

	return JB_OK;
}

jb_result_t jb_accumulate_inputs_decode(uint8_t **in_buff, uint64_t *in_remaining, jb_accumulate_inputs_t *out) {
	if (!out) {
		fprintf(stderr, "jb_accumulate_inputs_decode: out parameter is NULL\n");
		return JB_ERR_NO_OUT;
	}

	uint8_t *in_out_buff = *in_buff;
	uint64_t *in_out_remaining = in_remaining;
	jb_result_t result;

	uint64_t inputs_len;
	result = jb_codec_decode_general_int(&in_out_buff, in_out_remaining, &inputs_len);
	if (result != JB_OK) {
		fprintf(stderr, "jb_accumulate_inputs_decode: Couldn't decode inputs length, result: %u\n", result);
		return result;
	}

	out->inputs_len = inputs_len;
	out->inputs = malloc(inputs_len * sizeof(jb_accumulate_input_t));
	if (!out->inputs) {
		fprintf(stderr, "jb_accumulate_inputs_decode: Couldn't allocate memory for %lu inputs\n", inputs_len);
		return JB_ERR_MALLOC;
	}

	for (uint64_t i = 0; i < inputs_len; i++) {
		result = jb_accumulate_input_decode(&in_out_buff, in_out_remaining, &out->inputs[i]);
		if (result != JB_OK) {
			fprintf(stderr, "jb_accumulate_inputs_decode: Couldn't decode input %lu, result: %u\n", i, result);
			free(out->inputs);
			out->inputs = NULL;
			return result;
		}
	}

	*in_buff = in_out_buff;
	*in_remaining = *in_out_remaining;

	return JB_OK;
}
