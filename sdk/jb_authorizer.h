#pragma once

#include "host_functions.h"
#include "jb_service_types.h"
#include "host.h"

#include <stddef.h>

/// @brief You have to implement this for your authorizer to handle authorization requests
void jb_hook_is_authorized(jb_is_authorized_arguments_t*);

void return_to_host(const uint8_t *const ptr, const uint64_t len);
