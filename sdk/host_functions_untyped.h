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

// === Refine ===
/* 006 */ uint64_t jb_host_historical_lookup_untyped(uint64_t target_service_id, uint64_t hash_ptr, uint64_t out_ptr, uint64_t preimage_length);
/* 007 */ uint64_t jb_host_export_untyped(uint64_t export_ptr, uint64_t length);
/* 008 */ uint64_t jb_host_machine_untyped(uint64_t code_ptr, uint64_t code_length, uint64_t program_counter);
/* 009 */ uint64_t jb_host_peek_untyped(uint64_t machine_id, uint64_t out_ptr, uint64_t in_ptr, uint64_t length);
/* 010 */ uint64_t jb_host_poke_untyped(uint64_t machine_id, uint64_t in_ptr, uint64_t out_ptr, uint64_t length);
/* 011 */ uint64_t jb_host_pages_untyped(uint64_t machine_id, uint64_t start_page, uint64_t page_count, uint64_t mode);
/* 012 */ uint64_t jb_host_invoke_untyped(uint64_t machine_id, uint64_t parameters_ptr);
/* 013 */ uint64_t jb_host_expunge_untyped(uint64_t machine_id);

// === Accumulate ===
/* 014 */ uint64_t jb_host_bless_untyped(uint64_t manager, uint64_t assigners_ptr, uint64_t delegator, uint64_t registrar, uint64_t extra_ptr, uint64_t extra_count);
/* 015 */ uint64_t jb_host_assign_untyped(uint64_t core_index, uint64_t authorizers_ptr, uint64_t assigner);
/* 016 */ uint64_t jb_host_designate_untyped(uint64_t validators_ptr);
/* 017 */ uint64_t jb_host_checkpoint_untyped();
/* 018 */ uint64_t jb_host_new_untyped(uint64_t code_hash_ptr, uint64_t code_length, uint64_t min_gas_accumulate, uint64_t min_gas_memo, uint64_t gratis_storage_offset, uint64_t desired_id);
/* 019 */ uint64_t jb_host_upgrade_untyped(uint64_t code_hash_ptr, uint64_t min_gas_accumulate, uint64_t min_gas_memo);
/* 020 */ uint64_t jb_host_transfer_untyped(uint64_t receiver_id, uint64_t amount, uint64_t gas_limit, uint64_t memo_ptr);
/* 021 */ uint64_t jb_host_eject_untyped(uint64_t service_to_destroy, uint64_t memo_ptr);
/* 022 */ uint64_t jb_host_query_untyped(uint64_t hash_ptr, uint64_t preimage_length);
/* 023 */ uint64_t jb_host_solicit_untyped(uint64_t hash_ptr, uint64_t preimage_length);
/* 024 */ uint64_t jb_host_forget_untyped(uint64_t hash_ptr, uint64_t preimage_length);
/* 025 */ uint64_t jb_host_yield_untyped(uint64_t hash_ptr);
/* 026 */ uint64_t jb_host_provide_untyped(uint64_t service_id, uint64_t offset, uint64_t preimage_length);
