#pragma once

#include "host_functions.h"
#include "jb_service_types.h"
#include "jb_log.h"
#include "host.h"

/// @brief Get the chain parameters.
jb_chain_params_t jb_chain_params();

/// @brief Get the 32-byte chain entropy.
uint64_t jb_chain_entropy_32(uint8_t* entropy_32);

/// @brief Get the gas remaining of the invocation for the current service.
uint64_t jb_service_gas_remaining();

/// @brief Get the balance of the current service.
uint64_t jb_service_balance();

void jb_service_accumulate_operands();

/// @brief Get the info of the current service.
jb_service_info_t jb_service_info();

/// @brief Get the info of the given service id.
jb_service_info_t jb_service_info_of(uint64_t service_id);

/// @brief You have to implement this for your service to be able to refine
void jb_hook_refine(jb_refine_arguments_t*);

/// @brief You have to implement this for your service to be able to be accumulated
void jb_hook_accumulate(jb_accumulate_arguments_t*);

#define JAM_REG_0 "a0"
#define JAM_REG_1 "a1"
// #define JAM_REG_2 "a2"
// #define JAM_REG_3 "a3"
// #define JAM_REG_4 "a4"
// #define JAM_REG_5 "a5"
// #define JAM_REG_6 "a6"
// #define JAM_REG_7 "a7"
// #define JAM_REG_8 "a8"
// #define JAM_REG_9 "a9"
// #define JAM_REG_10 "a10"
// #define JAM_REG_11 "a11"
// #define JAM_REG_12 "a12"

void return_to_host(const uint8_t *const ptr, const uint64_t len);
