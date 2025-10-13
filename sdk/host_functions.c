#include "host_functions.h"
#include "host_functions_untyped.h"

#include <string.h>

char const* jb_host_result_name(uint64_t host_result) {
	switch (host_result) {
		case HOST_NONE:
			return "None";
		case HOST_WHAT:
			return "What";
		case HOST_OOB:
			return "Out of bounds";
		case HOST_WHO:
			return "Who?";
		case HOST_FULL:
			return "Full";
		case HOST_CORE:
			return "Core";
		case HOST_CASH:
			return "Cash";
		case HOST_LOW:
			return "Low";
		case HOST_HUH:
			return "Huh";
		case HOST_OK:
			return "Ok";
		default:
			return "Unknown host return";
	}
}

// === GENERAL ===

uint64_t jb_host_gas() {
	return jb_host_gas_untyped();
}

uint64_t jb_host_fetch(uint8_t* maybe_buffer, uint64_t offset, uint64_t buffer_len, uint64_t discriminator_w_10, uint64_t w_11, uint64_t w_12) {
	return jb_host_fetch_untyped((uint64_t)maybe_buffer, offset, buffer_len, discriminator_w_10, w_11, w_12);
}

uint64_t jb_host_read(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t out_offset, uint64_t out_len) {
	return jb_host_read_untyped(service_id, (uint64_t)key_ptr, key_len, (uint64_t)out_ptr, out_offset, out_len);
}

uint64_t jb_host_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len) {
	return jb_host_write_untyped((uint64_t)key_ptr, key_len, (uint64_t)value_ptr, value_len);
}

void jb_host_info(uint64_t service_id, jb_service_info_t* out_ptr) {
	jb_host_info_untyped(service_id, (uint64_t)out_ptr, 0, sizeof(jb_service_info_t));
}

// TODO make one abstraction without strlen
void jb_host_log(uint64_t level, uint8_t const* const target, uint64_t target_len, uint8_t const* const msg, uint64_t msg_len) {
	jb_host_log_untyped(level, (uint64_t)target, target_len, (uint64_t)msg, msg_len);
}

// === Accumulate ===

uint64_t jb_host_bless(uint64_t manager, uint8_t const* const assigners_ptr, uint64_t delegator, uint64_t registrar, uint8_t const* const extra_ptr, uint64_t extra_count) {
	return jb_host_bless_untyped(manager, (uint64_t)assigners_ptr, delegator, registrar, (uint64_t)extra_ptr, extra_count);
}

uint64_t jb_host_assign(uint64_t core_index, uint8_t const* const authorizers_ptr, uint64_t assigner) {
	return jb_host_assign_untyped(core_index, (uint64_t)authorizers_ptr, assigner);
}

uint64_t jb_host_designate(uint8_t const* const validators_ptr) {
	return jb_host_designate_untyped((uint64_t)validators_ptr);
}

uint64_t jb_host_checkpoint() {
	return jb_host_checkpoint_untyped();
}

uint64_t jb_host_new(const uint8_t *const code_hash_ptr, uint64_t code_length, uint64_t min_gas_accumulate, uint64_t min_gas_memo, uint64_t gratis_storage_offset, uint64_t desired_id) {
	return jb_host_new_untyped((uint64_t)code_hash_ptr, code_length, min_gas_accumulate, min_gas_memo, gratis_storage_offset, desired_id);
}

uint64_t jb_host_upgrade(const uint8_t *const code_hash_ptr, uint64_t min_gas_accumulate, uint64_t min_gas_memo) {
	return jb_host_upgrade_untyped((uint64_t)code_hash_ptr, min_gas_accumulate, min_gas_memo);
}

uint64_t jb_host_transfer(uint64_t receiver_id, uint64_t amount, uint64_t gas_limit, const uint8_t *const memo_ptr) {
	return jb_host_transfer_untyped(receiver_id, amount, gas_limit, (uint64_t)memo_ptr);
}
