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

void _jb_entry_refine(void *arg_ptr, uint64_t arg_len) {
	jb_init("Refine");

	jb_refine_arguments_t args;
	jb_result_t result = jb_refine_arguments_decode((uint8_t*)arg_ptr, arg_len, &args);
	if (result != JB_OK) {
		fprintf(stderr, "Could not decode refine args\n");
		return;
	}

	jb_hook_refine(&args);
}

void _jb_entry_accumulate(void* arg_ptr, uint64_t arg_len) {
	jb_init("Accumulate");
	
	jb_accumulate_arguments_t args;
    jb_result_t res = jb_accumulate_arguments_decode((uint8_t*)arg_ptr, arg_len, &args);
    if (res != JB_OK) {
        fprintf(stderr, "Could not decode accumulate args\n");
        return;
    }

	jb_hook_accumulate(&args);
}

POLKAVM_EXPORT(void, _jb_entry_refine, void*, uint64_t);
POLKAVM_EXPORT(void, _jb_entry_accumulate, void*, uint64_t);

// TODO still needed?
extern void _pvm_start(uint64_t *p);
