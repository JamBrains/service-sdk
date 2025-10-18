#include "jb_service.h"
#include "jb_system_check.h"
#include "jb_service_types.h"

#include <stddef.h>
#include <stdint.h>
#include <sys/auxv.h>
#include <stdio.h>
#include <stdlib.h>

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

void jb_service_accumulate_operands() {
	// TODO optimize by optimistically alloc 1KB on the stack, maybe it fits and if not then we still get the first part
	uint64_t encoded_len = jb_host_fetch(NULL, 0, 0, JB_FETCH_DISCRIMINATOR_INPUTS, 0, 0);
	if (encoded_len == 0)
		return;

	uint8_t* buff = (uint8_t*)malloc(encoded_len);
	if (!buff)
		return; // JB_ERR_MALLOC;

	// TODO handle err
	jb_host_fetch(NULL, 0, encoded_len, JB_FETCH_DISCRIMINATOR_INPUTS, 0, 0);

	free(buff);
}

jb_service_info_t jb_service_info_of(uint64_t service_id) {
	jb_service_info_t info;
	jb_host_info(service_id, &info);
	return info;
}

jb_service_info_t jb_service_info() {
	return jb_service_info_of(0xffffffffffffffff);
}

void return_to_host(const uint8_t *const ptr, const uint64_t len) {
	__asm__ volatile(
		"mv " JAM_REG_0 ", %0\n\t"
		"mv " JAM_REG_1 ", %1"
		:
		: "r"((uint64_t)ptr), "r"(len)
		: JAM_REG_0, JAM_REG_1);
}
