#include "host_functions_untyped.h"
#include "host.h"

#include <stdint.h>

// === General ===
/* GAS */ POLKAVM_IMPORT_WITH_INDEX(0, uint64_t /*gas*/, _jb_host_gas_untyped);
/* FETCH */ POLKAVM_IMPORT_WITH_INDEX(1, uint64_t /*gas*/, _jb_host_fetch_untyped, uint64_t /*buffer*/, uint64_t /*offset*/, uint64_t /*buffer_len*/, uint64_t /*discriminator_w_10*/, uint64_t /*w_11*/, uint64_t /*w_12*/);
/* LOOKUP */
/* READ */ POLKAVM_IMPORT_WITH_INDEX(3, uint64_t /* len_or_u64_max */, _jb_host_read_untyped, uint64_t /* service_id */, uint64_t /*key_ptr*/, uint64_t /*key_len*/, uint64_t /*out_ptr*/, uint64_t /*out_offset*/, uint64_t /*out_len*/);
/* WRITE */ POLKAVM_IMPORT_WITH_INDEX(4, uint64_t /*len_or_u64_max*/, _jb_host_write_untyped, uint64_t /*key_ptr*/, uint64_t /*key_len*/, uint64_t /*value_ptr*/, uint64_t /*value_len*/);
/* INFO */ POLKAVM_IMPORT_WITH_INDEX(5, uint64_t, _jb_host_info_untyped, uint64_t /*service_id*/, uint64_t /*out_ptr*/, uint64_t /*offset*/, uint64_t /*length*/);
/* LOG */ POLKAVM_IMPORT_WITH_INDEX(100, uint64_t, _jb_host_log_untyped, uint64_t /*level*/, uint64_t /*target*/, uint64_t /*target_len*/, uint64_t /*msg*/, uint64_t /*msg_len*/);

// === Refine ===

/* HISTORICAL_LOOKUP */ POLKAVM_IMPORT_WITH_INDEX(6, uint64_t, _jb_host_historical_lookup_untyped, uint64_t /*target_service_id*/, uint64_t /*hash_ptr*/, uint64_t /*out_ptr*/, uint64_t /*preimage_length*/);
/* EXPORT */ POLKAVM_IMPORT_WITH_INDEX(7, uint64_t, _jb_host_export_untyped, uint64_t /*export_ptr*/, uint64_t /*length*/);
/* MACHINE */ POLKAVM_IMPORT_WITH_INDEX(8, uint64_t, _jb_host_machine_untyped, uint64_t /*code_ptr*/, uint64_t /*code_length*/, uint64_t /*program_counter*/);
/* PEEK */ POLKAVM_IMPORT_WITH_INDEX(9, uint64_t, _jb_host_peek_untyped, uint64_t /*machine_id*/, uint64_t /*out_ptr*/, uint64_t /*in_ptr*/, uint64_t /*length*/);
/* POKE */ POLKAVM_IMPORT_WITH_INDEX(10, uint64_t, _jb_host_poke_untyped, uint64_t /*machine_id*/, uint64_t /*in_ptr*/, uint64_t /*out_ptr*/, uint64_t /*length*/);
/* PAGES */ POLKAVM_IMPORT_WITH_INDEX(11, uint64_t, _jb_host_pages_untyped, uint64_t /*machine_id*/, uint64_t /*start_page*/, uint64_t /*page_count*/, uint64_t /*mode*/);
/* INVOKE */ POLKAVM_IMPORT_WITH_INDEX(12, uint64_t, _jb_host_invoke_untyped, uint64_t /*machine_id*/, uint64_t /*parameters_ptr*/);
/* EXPUNGE */ POLKAVM_IMPORT_WITH_INDEX(13, uint64_t, _jb_host_expunge_untyped, uint64_t /*machine_id*/);

// === Accumulate ===
/* BLESS */ POLKAVM_IMPORT_WITH_INDEX(14, uint64_t, _jb_host_bless_untyped, uint64_t /*manager*/, uint64_t /*assigners_ptr*/, uint64_t /*delegator*/, uint64_t /*registrar*/, uint64_t /*extra_ptr*/, uint64_t /*extra_count*/);
/* ASSIGN */ POLKAVM_IMPORT_WITH_INDEX(15, uint64_t, _jb_host_assign_untyped, uint64_t /*core_index*/, uint64_t /*address*/, uint64_t /*assigner*/);
/* DESIGNATE */ POLKAVM_IMPORT_WITH_INDEX(16, uint64_t, _jb_host_designate_untyped, uint64_t /*validators_ptr*/);
/* CHECKPOINT */ POLKAVM_IMPORT_WITH_INDEX(17, uint64_t, _jb_host_checkpoint_untyped);
/* NEW */ POLKAVM_IMPORT_WITH_INDEX(18, uint64_t, _jb_host_new_untyped, uint64_t /*code_hash_ptr*/, uint64_t /*code_length*/, uint64_t /*min_gas_accumulate*/, uint64_t /*min_gas_memo*/, uint64_t /*gratis_storage_offset*/, uint64_t /*desired_id*/);
/* UPGRADE */ POLKAVM_IMPORT_WITH_INDEX(19, uint64_t, _jb_host_upgrade_untyped, uint64_t /*code_hash_ptr*/, uint64_t /*min_gas_accumulate*/, uint64_t /*min_gas_memo*/);
/* TRANSFER */ POLKAVM_IMPORT_WITH_INDEX(20, uint64_t, _jb_host_transfer_untyped, uint64_t /*receiver_id*/, uint64_t /*amount*/, uint64_t /*gas_limit*/, uint64_t /*memo_ptr*/);
/* EJECT */ POLKAVM_IMPORT_WITH_INDEX(21, uint64_t, _jb_host_eject_untyped, uint64_t /*service_to_destroy*/, uint64_t /*memo_ptr*/);
/* QUERY */ POLKAVM_IMPORT_WITH_INDEX(22, uint64_t, _jb_host_query_untyped, uint64_t /*hash_ptr*/, uint64_t /*preimage_length*/);
/* SOLICIT */ POLKAVM_IMPORT_WITH_INDEX(23, uint64_t, _jb_host_solicit_untyped, uint64_t /*hash_ptr*/, uint64_t /*preimage_length*/);
/* FORGET */ POLKAVM_IMPORT_WITH_INDEX(24, uint64_t, _jb_host_forget_untyped, uint64_t /*hash_ptr*/, uint64_t /*preimage_length*/);
/* YIELD */ POLKAVM_IMPORT_WITH_INDEX(25, uint64_t, _jb_host_yield_untyped, uint64_t /*hash_ptr*/);
/* PROVIDE */ POLKAVM_IMPORT_WITH_INDEX(26, uint64_t, _jb_host_provide_untyped, uint64_t /*service_id*/, uint64_t /*offset*/, uint64_t /*preimage_length*/);

// === General ===

uint64_t jb_host_gas_untyped() {
    return _jb_host_gas_untyped();
}

uint64_t jb_host_fetch_untyped(uint64_t buffer, uint64_t offset, uint64_t buffer_len, uint64_t discriminator_w_10, uint64_t w_11, uint64_t w_12) {
    return _jb_host_fetch_untyped(buffer, offset, buffer_len, discriminator_w_10, w_11, w_12);
}

// TODO lookup

uint64_t jb_host_read_untyped(uint64_t service_id, uint64_t key_ptr, uint64_t key_len, uint64_t out_ptr, uint64_t out_offset, uint64_t out_len) {
    return _jb_host_read_untyped(service_id, key_ptr, key_len, out_ptr, out_offset, out_len);
}

uint64_t jb_host_write_untyped(uint64_t key_ptr, uint64_t key_len, uint64_t value_ptr, uint64_t value_len) {
    return _jb_host_write_untyped(key_ptr, key_len, value_ptr, value_len);
}

uint64_t jb_host_info_untyped(uint64_t service_id, uint64_t out_ptr, uint64_t offset, uint64_t length) {
    return _jb_host_info_untyped(service_id, (uint64_t)out_ptr, offset, length);
}

uint64_t jb_host_log_untyped(uint64_t level, uint64_t target, uint64_t target_len, uint64_t msg, uint64_t msg_len) {
    return _jb_host_log_untyped(level, target, target_len, msg, msg_len);
}

// === Refine ===

uint64_t jb_host_historical_lookup_untyped(uint64_t target_service_id, uint64_t hash_ptr, uint64_t out_ptr, uint64_t preimage_length) {
    return _jb_host_historical_lookup_untyped(target_service_id, hash_ptr, out_ptr, preimage_length);
}

uint64_t jb_host_export_untyped(uint64_t export_ptr, uint64_t length) {
    return _jb_host_export_untyped(export_ptr, length);
}

uint64_t jb_host_machine_untyped(uint64_t code_ptr, uint64_t code_length, uint64_t program_counter) {
    return _jb_host_machine_untyped(code_ptr, code_length, program_counter);
}

uint64_t jb_host_peek_untyped(uint64_t machine_id, uint64_t out_ptr, uint64_t in_ptr, uint64_t length) {
    return _jb_host_peek_untyped(machine_id, out_ptr, in_ptr, length);
}

uint64_t jb_host_poke_untyped(uint64_t machine_id, uint64_t in_ptr, uint64_t out_ptr, uint64_t length) {
    return _jb_host_poke_untyped(machine_id, in_ptr, out_ptr, length);
}

uint64_t jb_host_pages_untyped(uint64_t machine_id, uint64_t start_page, uint64_t page_count, uint64_t mode) {
    return _jb_host_pages_untyped(machine_id, start_page, page_count, mode);
}

uint64_t jb_host_invoke_untyped(uint64_t machine_id, uint64_t parameters_ptr) {
    return _jb_host_invoke_untyped(machine_id, parameters_ptr);
}

uint64_t jb_host_expunge_untyped(uint64_t machine_id) {
    return _jb_host_expunge_untyped(machine_id);
}

// === Accumulate ===

uint64_t jb_host_bless_untyped(uint64_t manager, uint64_t assigners_ptr, uint64_t delegator, uint64_t registrar, uint64_t extra_ptr, uint64_t extra_count) {
    return _jb_host_bless_untyped(manager, assigners_ptr, delegator, registrar, extra_ptr, extra_count);
}

uint64_t jb_host_assign_untyped(uint64_t core_index, uint64_t authorizers_ptr, uint64_t assigner) {
    return _jb_host_assign_untyped(core_index, authorizers_ptr, assigner);
}

uint64_t jb_host_designate_untyped(uint64_t validators_ptr) {
    return _jb_host_designate_untyped(validators_ptr);
}

uint64_t jb_host_checkpoint_untyped() {
    return _jb_host_checkpoint_untyped();
}

uint64_t jb_host_new_untyped(uint64_t code_hash_ptr, uint64_t code_length, uint64_t min_gas_accumulate, uint64_t min_gas_memo, uint64_t gratis_storage_offset, uint64_t desired_id) {
    return _jb_host_new_untyped(code_hash_ptr, code_length, min_gas_accumulate, min_gas_memo, gratis_storage_offset, desired_id);
}

uint64_t jb_host_upgrade_untyped(uint64_t code_hash_ptr, uint64_t min_gas_accumulate, uint64_t min_gas_memo) {
    return _jb_host_upgrade_untyped(code_hash_ptr, min_gas_accumulate, min_gas_memo);
}

uint64_t jb_host_transfer_untyped(uint64_t receiver_id, uint64_t amount, uint64_t gas_limit, uint64_t memo_ptr) {
    return _jb_host_transfer_untyped(receiver_id, amount, gas_limit, memo_ptr);
}

uint64_t jb_host_eject_untyped(uint64_t service_to_destroy, uint64_t memo_ptr) {
    return _jb_host_eject_untyped(service_to_destroy, memo_ptr);
}

uint64_t jb_host_query_untyped(uint64_t hash_ptr, uint64_t preimage_length) {
    return _jb_host_query_untyped(hash_ptr, preimage_length);
}

uint64_t jb_host_solicit_untyped(uint64_t hash_ptr, uint64_t preimage_length) {
    return _jb_host_solicit_untyped(hash_ptr, preimage_length);
}

uint64_t jb_host_forget_untyped(uint64_t hash_ptr, uint64_t preimage_length) {
    return _jb_host_forget_untyped(hash_ptr, preimage_length);
}

uint64_t jb_host_yield_untyped(uint64_t hash_ptr) {
    return _jb_host_yield_untyped(hash_ptr);
}

uint64_t jb_host_provide_untyped(uint64_t service_id, uint64_t offset, uint64_t preimage_length) {
    return _jb_host_provide_untyped(service_id, offset, preimage_length);
}
