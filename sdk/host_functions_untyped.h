#pragma once

#include <stdint.h>

#include "host.h"

/**
Untyped host functions.

They always assume that every argument is a uint64 and that the return value as well. Note that the return value may be undefined for some of them.
*/

// === General ===
/* 000 */ uint64_t jb_host_gas_untyped();
/* 001 */ uint64_t jb_host_fetch_untyped(uint64_t buffer, uint64_t offset, uint64_t buffer_len, uint64_t discriminator_w_10, uint64_t w_11, uint64_t w_12);
/* 003 */ uint64_t jb_host_read_untyped(uint64_t service_id, uint64_t key_ptr, uint64_t key_len, uint64_t out_ptr, uint64_t out_offset, uint64_t out_len);
/* 004 */ uint64_t jb_host_write_untyped(uint64_t key_ptr, uint64_t key_len, uint64_t value_ptr, uint64_t value_len);
/* 005 */ uint64_t jb_host_info_untyped(uint64_t service_id, uint64_t out_ptr, uint64_t offset, uint64_t length);
/* 100 */ uint64_t jb_host_log_untyped(uint64_t level, uint64_t target, uint64_t target_len, uint64_t msg, uint64_t msg_len);

// === Accumulate ===
/* 014 */ uint64_t jb_host_bless_untyped(uint64_t manager, uint64_t assigners_ptr, uint64_t delegator, uint64_t registrar, uint64_t extra_ptr, uint64_t extra_count);
/* 015 */ uint64_t jb_host_assign_untyped(uint64_t core_index, uint64_t authorizers_ptr, uint64_t assigner);
/* 016 */ uint64_t jb_host_designate_untyped(uint64_t validators_ptr);
/* 017 */ uint64_t jb_host_checkpoint_untyped();
