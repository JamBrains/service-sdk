#include "host_functions.h"
#include "host_functions_untyped.h"

#include <string.h>

// Optimized version using static lookup table
char const* jb_host_result_name(uint64_t host_result) {
    static const char* const result_names[] = {
        "None",           // HOST_NONE = 0
        "What",          // HOST_WHAT = 1
        "Out of bounds", // HOST_OOB = 2
        "Who?",          // HOST_WHO = 3
        "Full",          // HOST_FULL = 4
        "Core",          // HOST_CORE = 5
        "Cash",          // HOST_CASH = 6
        "Low",           // HOST_LOW = 7
        "Huh",           // HOST_HUH = 8
        "Ok"            // HOST_OK = 9
    };
    static const size_t result_names_size = sizeof(result_names) / sizeof(result_names[0]);
    
    return (host_result < result_names_size) ? result_names[host_result] : "Unknown host return";
}

void assert_host_ok(uint64_t result) {
    // Using bit manipulation for faster check
    // This assumes HOST_OK is greater than other error codes
    // and error codes are sequential from 0 to HOST_HUH
    if (result <= HOST_HUH) {
        POLKAVM_TRAP();
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

// === Refine ===

uint64_t jb_host_historical_lookup(uint64_t target_service_id, uint8_t const* const hash_ptr, uint8_t* out_ptr, uint64_t preimage_length) {
	return jb_host_historical_lookup_untyped(target_service_id, (uint64_t)hash_ptr, (uint64_t)out_ptr, preimage_length);
}

uint64_t jb_host_export(uint8_t const* const export_ptr, uint64_t length) {
	return jb_host_export_untyped((uint64_t)export_ptr, length);
}

uint64_t jb_host_machine(uint8_t const* const code_ptr, uint64_t code_length, uint64_t program_counter) {
	return jb_host_machine_untyped((uint64_t)code_ptr, code_length, program_counter);
}

uint64_t jb_host_peek(uint64_t machine_id, uint8_t* out_ptr, uint8_t const* const in_ptr, uint64_t length) {
	return jb_host_peek_untyped(machine_id, (uint64_t)out_ptr, (uint64_t)in_ptr, length);
}

uint64_t jb_host_poke(uint64_t machine_id, uint8_t const* const in_ptr, uint8_t* out_ptr, uint64_t length) {
	return jb_host_poke_untyped(machine_id, (uint64_t)in_ptr, (uint64_t)out_ptr, length);
}

uint64_t jb_host_pages(uint64_t machine_id, uint64_t start_page, uint64_t page_count, uint64_t mode) {
	return jb_host_pages_untyped(machine_id, start_page, page_count, mode);
}

uint64_t jb_host_invoke(uint64_t machine_id, uint8_t const* const parameters_ptr) {
	return jb_host_invoke_untyped(machine_id, (uint64_t)parameters_ptr);
}

uint64_t jb_host_expunge(uint64_t machine_id) {
	return jb_host_expunge_untyped(machine_id);
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

uint64_t jb_host_eject(uint64_t service_to_destroy, const uint8_t *const memo_ptr) {
	return jb_host_eject_untyped(service_to_destroy, (uint64_t)memo_ptr);
}

uint64_t jb_host_query(const uint8_t *const hash_ptr, uint64_t preimage_length) {
	return jb_host_query_untyped((uint64_t)hash_ptr, preimage_length);
}

uint64_t jb_host_solicit(const uint8_t *const hash_ptr, uint64_t preimage_length) {
	return jb_host_solicit_untyped((uint64_t)hash_ptr, preimage_length);
}

uint64_t jb_host_forget(const uint8_t *const hash_ptr, uint64_t preimage_length) {
	return jb_host_forget_untyped((uint64_t)hash_ptr, preimage_length);
}

uint64_t jb_host_yield(const uint8_t *const hash_ptr) {
	return jb_host_yield_untyped((uint64_t)hash_ptr);
}

uint64_t jb_host_provide(uint64_t service_id, uint64_t offset, uint64_t preimage_length) {
	return jb_host_provide_untyped(service_id, offset, preimage_length);
}
