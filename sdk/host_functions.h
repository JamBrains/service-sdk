#pragma once 

#include <stdint.h>
#include "jb_service_types.h"

uint64_t jb_host_gas();

#define JB_FETCH_DISCRIMINATOR_CHAIN_PARAMS 0
#define JB_FETCH_DISCRIMINATOR_CHAIN_ENTROPY32 1
#define JB_FETCH_DISCRIMINATOR_OPERANDS 14

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

// === General ===

uint64_t jb_host_fetch(uint8_t *maybe_buffer, uint64_t offset, uint64_t buffer_len, jb_fetch_discriminator_t discriminator, uint64_t w_11, uint64_t w_12);

uint64_t jb_host_read(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t out_offset, uint64_t out_len);

uint64_t jb_host_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len);

void jb_host_info(uint64_t service_id, jb_service_info_t* out_ptr);

void jb_host_log(uint64_t level, uint8_t const* const target, uint64_t target_len, uint8_t const* const msg, uint64_t msg_len);

// === Accumulate ===

uint64_t jb_host_bless(uint64_t manager, uint8_t const* const assigners_ptr, uint64_t delegator, uint64_t registrar, uint8_t const* const extra_ptr, uint64_t extra_count);

uint64_t jb_host_assign(uint64_t core_index, uint8_t const* const authorizers_ptr, uint64_t assigner);

uint64_t jb_host_designate(uint8_t const* const validators_ptr);

uint64_t jb_host_checkpoint();
