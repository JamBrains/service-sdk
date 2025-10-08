#include "jb_result.h"
#include "jb_service_types.h"
#include "jb_system_check.h"
#include "jb_service.h"
#include "host.h"

#include <stdio.h>

// To make the C compiler happy
int main(int _argc, char **_argv)
{
	POLKAVM_TRAP();
}

// prelude
void jb_init(char const* const name) {
	extern char const* log_target;
	log_target = name;
	jb_log_info(name, "Initializing...");
	jb_system_check_full();
}

void _jb_entry_on_transfer()
{
	POLKAVM_TRAP();
}

void _jb_entry_is_authorized() {
	jb_hook_is_authorized();
}

void _jb_entry_refine() {
	jb_hook_refine();
}

void _jb_entry_accumulate(void* arg_ptr, uint64_t arg_len) {
	jb_init("JB Service");
	
	jb_accumulate_arguments_t args;
    jb_result_t res = jb_accumulate_arguments_decode((uint8_t*)arg_ptr, arg_len, &args);
    if (res != JB_OK) {
        fprintf(stderr, "Could not decode accumulate args\n");
        return;
    }

	jb_hook_accumulate(&args);
}

POLKAVM_EXPORT(void, _jb_entry_on_transfer);
POLKAVM_EXPORT(void, _jb_entry_is_authorized);
POLKAVM_EXPORT(void, _jb_entry_refine);
POLKAVM_EXPORT(void, _jb_entry_accumulate, void*, uint64_t);

// TODO still needed?
extern void _pvm_start(uint64_t *p);

// unused
/*

#define JAM_REG_0 "a0"
#define JAM_REG_1 "a1"
#define JAM_REG_2 "a2"
#define JAM_REG_3 "a3"
#define JAM_REG_4 "a4"
#define JAM_REG_5 "a5"
#define JAM_REG_6 "a6"
#define JAM_REG_7 "a7"
#define JAM_REG_8 "a8"
#define JAM_REG_9 "a9"
#define JAM_REG_10 "a10"
#define JAM_REG_11 "a11"
#define JAM_REG_12 "a12"

void return_to_host(const uint8_t *const data, const uint64_t len) {
	__asm__ volatile(
		"mv " JAM_REG_0 ", %0\n\t"
		"mv " JAM_REG_1 ", %1"
		:
		: "r"(data), "r"(len)
		: JAM_REG_0, JAM_REG_1);
}

*/
