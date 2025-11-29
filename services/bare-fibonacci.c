/**

Toggle the "exclude entry.c" in the UI when you compile this.

This file demonstrates on how to use the JamBrains Service SDK toolchain without the actual SDK.
The SDK normally runs some startup code at the beginning to sanity check the environment and some
other things like loading the accumulation arguments. You can also live without if you want and
directly implement the hooks that Polkatool will expose.

*/
#include "host.h"

#include <stdint.h>

// Write some data to register 7 in order to return it to the host.
void write_register_7(const uint64_t value) {
	__asm__ volatile("mv a0, %0" :: "r"(value) : "a0");
}

uint64_t fibonacci(const uint64_t n) {
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void _jb_entry_accumulate(void* arg_ptr, uint64_t arg_len) {
    uint64_t result = fibonacci(10);
    write_register_7(result);
}

void _jb_entry_refine(void *arg_ptr, uint64_t arg_len) {
}


// To make the C compiler happy
int main(int _argc, char **_argv)
{
    POLKAVM_TRAP();
}

// Tell Polkatool about our entry points
POLKAVM_EXPORT(void, _jb_entry_refine, void*, uint64_t);
POLKAVM_EXPORT(void, _jb_entry_accumulate, void*, uint64_t);
