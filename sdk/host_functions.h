#pragma once 

#include <stdint.h>
#include "jb_service_types.h"
#include "jb_storage.h"

uint64_t jb_host_gas();

#define JB_FETCH_DISCRIMINATOR_CHAIN_PARAMS 0
#define JB_FETCH_DISCRIMINATOR_CHAIN_ENTROPY32 1
#define JB_FETCH_DISCRIMINATOR_NTH_ITEM_PAYLOAD 13
#define JB_FETCH_DISCRIMINATOR_INPUTS 14

/// W_10 register discriminator for the  fetch host call.
typedef uint64_t jb_fetch_discriminator_t;

#define HOST_NONE ((uint64_t)-1)
#define HOST_WHAT ((uint64_t)-2)
#define HOST_OOB ((uint64_t)-3)
#define HOST_WHO ((uint64_t)-4)
#define HOST_FULL ((uint64_t)-5)
#define HOST_CORE ((uint64_t)-6)
#define HOST_CASH ((uint64_t)-7)
#define HOST_LOW ((uint64_t)-8)
#define HOST_HUH ((uint64_t)-9)

#define HOST_OK ((uint64_t)0)

char const* jb_host_result_name(uint64_t host_result);
void assert_host_ok(uint64_t);

// === General ===

uint64_t jb_host_fetch(uint8_t *maybe_buffer, uint64_t offset, uint64_t buffer_len, jb_fetch_discriminator_t discriminator, uint64_t w_11, uint64_t w_12);
uint64_t jb_host_read(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t out_offset, uint64_t out_len);
uint64_t jb_host_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len);
void jb_host_info(uint64_t service_id, jb_service_info_t* out_ptr);
void jb_host_log(uint64_t level, uint8_t const* const target, uint64_t target_len, uint8_t const* const msg, uint64_t msg_len);

// === Refine ===

uint64_t jb_host_historical_lookup(uint64_t target_service_id, uint8_t const* const hash_ptr, uint8_t* out_ptr, uint64_t preimage_length);
uint64_t jb_host_export(uint8_t const* const export_ptr, uint64_t length);
uint64_t jb_host_machine(uint8_t const* const code_ptr, uint64_t code_length, uint64_t program_counter);
uint64_t jb_host_peek(uint64_t machine_id, uint8_t* out_ptr, uint8_t const* const in_ptr, uint64_t length);
uint64_t jb_host_poke(uint64_t machine_id, uint8_t const* const in_ptr, uint8_t* out_ptr, uint64_t length);
uint64_t jb_host_pages(uint64_t machine_id, uint64_t start_page, uint64_t page_count, uint64_t mode);
uint64_t jb_host_invoke(uint64_t machine_id, uint8_t const* const parameters_ptr);
uint64_t jb_host_expunge(uint64_t machine_id);

// === Accumulate ===

uint64_t jb_host_bless(uint64_t manager, uint8_t const* const assigners_ptr, uint64_t delegator, uint64_t registrar, uint8_t const* const extra_ptr, uint64_t extra_count);
uint64_t jb_host_assign(uint64_t core_index, uint8_t const* const authorizers_ptr, uint64_t assigner);
uint64_t jb_host_designate(uint8_t const* const validators_ptr);
uint64_t jb_host_checkpoint();
uint64_t jb_host_new(uint8_t const* const code_hash_ptr, uint64_t code_length, uint64_t min_gas_accumulate, uint64_t min_gas_memo, uint64_t gratis_storage_offset, uint64_t desired_id);
uint64_t jb_host_upgrade(uint8_t const* const code_hash_ptr, uint64_t min_gas_accumulate, uint64_t min_gas_memo);
uint64_t jb_host_transfer(uint64_t receiver_id, uint64_t amount, uint64_t gas_limit, uint8_t const* const memo_ptr);
uint64_t jb_host_eject(uint64_t service_to_destroy, uint8_t const* const memo_ptr);
uint64_t jb_host_query(uint8_t const* const hash_ptr, uint64_t preimage_length);
uint64_t jb_host_solicit(uint8_t const* const hash_ptr, uint64_t preimage_length);
uint64_t jb_host_forget(uint8_t const* const hash_ptr, uint64_t preimage_length);
uint64_t jb_host_yield(uint8_t const* const hash_ptr);
uint64_t jb_host_provide(uint64_t service_id, uint64_t offset, uint64_t preimage_length);
