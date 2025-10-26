#include "host_functions_untyped.h"
#include "jb_assert.h"
#include "jb_system_check.h"
#include "jb_service_types.h"

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

void jb_system_check_full() {
	jb_system_check_types();
	jb_system_check_fetch();
	jb_system_check_fpu();
	jb_system_check_memory();
	jb_system_check_sbrk();
}

void jb_system_check_types() {
	static_assert(sizeof(bool) == 1, "bool is not 1 byte");
	static_assert(sizeof(char) == 1, "char is not 1 byte");
	static_assert(sizeof(short) == 2, "short is not 2 bytes");
	static_assert(sizeof(int) == 4, "int is not 4 bytes");
	static_assert(sizeof(long) == 8, "long is not 8 bytes");
	static_assert(sizeof(long long) == 8, "long long is not 8 bytes");
	static_assert(sizeof(void *) == 8, "void* is not 8 bytes");

	static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");
	static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");
	static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");
	static_assert(sizeof(uint64_t) == 8, "uint64_t is not 8 bytes");

	static_assert(sizeof(jb_service_info_t) == JB_SERVICE_INFO_SIZE, "jb_service_info_t is not JB_SERVICE_INFO_SIZE bytes");
	static_assert(sizeof(jb_chain_params_t) == JB_CHAIN_PARAMS_SIZE, "jb_chain_params_t is not JB_CHAIN_PARAMS_SIZE bytes");

	static_assert(sizeof(float) == 4, "float is not 4 bytes");
	static_assert(sizeof(double) == 8, "double is not 8 bytes");
	static_assert(sizeof(long double) == 16, "long double is not 16 bytes");
}

void jb_system_check_fetch() {
	uint64_t chain_params_len = jb_host_fetch_untyped(0, 0, 0, 0, 0, 0);
	jb_assert_equal(chain_params_len, JB_CHAIN_PARAMS_SIZE, "chain_params_len should be JB_CHAIN_PARAMS_SIZE");
}

void jb_system_check_fpu() {
	jb_assert(FLT_EPSILON > 0, "FLT_EPSILON is not greater than 0");

	float a = 2.0f, b = 3.0f, c = 5.0f;
	jb_assert(fabs((a + b) - c) < FLT_EPSILON, "Basic addition failed");
	jb_assert(fabs((c - b) - a) < FLT_EPSILON, "Basic subtraction failed");
	jb_assert(fabs((a * b) - 6.0f) < FLT_EPSILON, "Basic multiplication failed");
	jb_assert(fabs((c / a) - 2.5f) < FLT_EPSILON, "Basic division failed");

	float tiny = FLT_MIN / 2.0f;
	if (tiny > 0.0f)
	{ // Subnormal stuff
		jb_assert(tiny < FLT_MIN, "Subnormal number handling broken");
		jb_assert(tiny + tiny == FLT_MIN, "Subnormal arithmetic broken");
	}
}

void jb_system_check_memory() {
	const int check_size = 10000;
	char* volatile p = malloc(check_size);
	jb_assert(p != NULL, "malloc failed");

	for (size_t i = 0; i < check_size; i++) {
		p[i] = i;
		jb_assert_equal(p[i], i % 256, "Memory is not writeable");
	}

	free(p);
}

// SBRK instruction is a custom instruction "0x0b, 1, 0" just for PVM.
static inline void *sbrk_custom(size_t size)
{
	void *address;
	__asm__ volatile(
		".insn r 0xb, 1, 0, %0, %1, zero"
		: "=r"(address) // output: address goes to a register
		: "r"(size)		// input: size from a register
		: "memory"		// clobber: may modify memory
	);
	return address;
}

void jb_system_check_sbrk() {
	// Calling sbrk twice with zero should return the same pointer
	void* base1 = sbrk_custom(0);
	void* base2 = sbrk_custom(0);
	jb_assert(base1 == base2 && base1 != NULL, "sbrk(0) should return the same pointer");
}
