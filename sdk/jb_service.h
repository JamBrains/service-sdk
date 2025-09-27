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

/// @brief Get the info of the current service.
jb_service_info_t jb_service_info();

/// @brief Get the info of the given service id.
jb_service_info_t jb_service_info_of(uint64_t service_id);

/// @brief You have to implement this for your service to be able to receive transfers
void jb_hook_on_transfer(char** _out_ptr, uint64_t* _out_len);

/// @brief You have to implement this for your service to act as an authorizer
void jb_hook_is_authorized();

/// @brief You have to implement this for your service to be able to refine
void jb_hook_refine();

/// @brief You have to implement this for your service to be able to be accumulated
void jb_hook_accumulate(jb_accumulate_arguments_t*);
