#include "jb_util.h"

static char const hex_digits[] = "0123456789abcdef";

uint64_t jb_util_bin2hex(char const* const bytes, uint64_t bytes_len, char* out_ptr, uint64_t out_len) {
    uint64_t i = 0;

	while (i < bytes_len && i * 2 < out_len) {
		out_ptr[i * 2] = hex_digits[(bytes[i] >> 4) & 0x0F];
		out_ptr[i * 2 + 1] = hex_digits[bytes[i] & 0x0F];
		i++;
	}

	return i * 2;
}
