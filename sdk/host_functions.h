#pragma once 

#include <stdint.h>
#include "jb_service_types.h"

uint64_t jb_host_gas();

#define JB_FETCH_DISCRIMINATOR_CHAIN_PARAMS 0
#define JB_FETCH_DISCRIMINATOR_CHAIN_ENTROPY32 1

/// W_10 register discriminator for the  fetch host call.
typedef uint64_t jb_fetch_discriminator_t;

uint64_t jb_host_fetch(uint8_t *maybe_buffer, uint64_t offset, uint64_t buffer_len, jb_fetch_discriminator_t discriminator, uint64_t w_11, uint64_t w_12);

uint64_t jb_host_read(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t out_offset, uint64_t out_len);

uint64_t jb_host_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len);

void jb_host_info(uint64_t service_id, jb_service_info_t* out_ptr);

void jb_host_bless(uint64_t manager, uint8_t const* const assigners_ptr, uint64_t delegator, uint64_t registrar, uint8_t const* const extra_ptr, uint64_t extra_count);

void jb_host_log(uint64_t level, uint8_t const* const target, uint64_t target_len, uint8_t const* const msg, uint64_t msg_len);
