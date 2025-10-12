#include "host_functions.h"
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

// === Accumulate ===
/* BLESS */ POLKAVM_IMPORT_WITH_INDEX(14, uint64_t, _jb_host_bless_untyped, uint64_t /*manager*/, uint64_t /*assigners_ptr*/, uint64_t /*delegator*/, uint64_t /*registrar*/, uint64_t /*extra_ptr*/, uint64_t /*extra_count*/);
/* ASSIGN */ POLKAVM_IMPORT_WITH_INDEX(15, uint64_t, _jb_host_assign_untyped, uint64_t /*core_index*/, uint64_t /*address*/, uint64_t /*assigner*/);
/* DESIGNATE */ POLKAVM_IMPORT_WITH_INDEX(16, uint64_t, _jb_host_designate_untyped, uint64_t /*validators_ptr*/);


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

uint64_t jb_host_bless_untyped(uint64_t manager, uint64_t assigners_ptr, uint64_t delegator, uint64_t registrar, uint64_t extra_ptr, uint64_t extra_count) {
    return _jb_host_bless_untyped(manager, assigners_ptr, delegator, registrar, extra_ptr, extra_count);
}

uint64_t jb_host_assign_untyped(uint64_t core_index, uint64_t authorizers_ptr, uint64_t assigner) {
    return _jb_host_assign_untyped(core_index, authorizers_ptr, assigner);
}

uint64_t jb_host_designate_untyped(uint64_t validators_ptr) {
    return _jb_host_designate_untyped(validators_ptr);
}
