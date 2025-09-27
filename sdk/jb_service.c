#include "jb_service.h"
#include "jb_system_check.h"
#include "jb_service_types.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/auxv.h>
#include <stdio.h>

jb_chain_params_t jb_chain_params() {
	jb_chain_params_t params;
	jb_host_fetch((uint8_t*)&params, 0, JB_CHAIN_PARAMS_SIZE, JB_FETCH_DISCRIMINATOR_CHAIN_PARAMS, 0, 0);
	return params;
}

uint64_t jb_chain_entropy_32(uint8_t* entropy_32) {
	return jb_host_fetch((uint8_t*)entropy_32, 0, 32, JB_FETCH_DISCRIMINATOR_CHAIN_ENTROPY32, 0, 0);
}

uint64_t jb_service_gas_remaining() {
	return jb_host_gas();
}

uint64_t jb_service_balance() {
	return jb_service_info().balance;
}

jb_service_info_t jb_service_info_of(uint64_t service_id) {
	jb_service_info_t info;
	jb_host_info(service_id, &info);
	return info;
}

jb_service_info_t jb_service_info() {
	return jb_service_info_of(0xffffffffffffffff);
}
