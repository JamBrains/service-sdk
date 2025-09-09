#include "jb_storage.h"
#include "host_functions.h"

#include <string.h>
#include <stdio.h>

// Host return codes, must match up with GP 0.6.7
/*@host_fn_return_name_to_number %{
    :none => 2 ** 64 - 1,
    :what => 2 ** 64 - 2,
    :oob => 2 ** 64 - 3,
    :who => 2 ** 64 - 4,
    :full => 2 ** 64 - 5,
    :core => 2 ** 64 - 6,
    :cash => 2 ** 64 - 7,
    :low => 2 ** 64 - 8,
    :huh => 2 ** 64 - 9,
    :ok => 0,
    # Inner ones
    :halt => 0,
    :panic => 1,
    :fault => 2,
    :host => 3,
    # Out Of Gas
    :oog => 4
  }*/

#define HOST_NONE ((uint64_t)-1)
#define HOST_WHAT ((uint64_t)-2)
#define HOST_OOB ((uint64_t)-3)
#define HOST_WHO ((uint64_t)-4)
#define HOST_FULL ((uint64_t)-5)
#define HOST_CORE ((uint64_t)-6)
#define HOST_CASH ((uint64_t)-7)
#define HOST_LOW ((uint64_t)-8)
#define HOST_HUH ((uint64_t)-9)

#define HOST_OK ((uint64_t)0)

jb_result_t jb_storage_kv_read(uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len) {
	return jb_storage_kv_read_of(JB_SERVICE_ID_SELF, key_ptr, key_len, out_ptr, value_offset, value_max_len, out_value_len);
}

jb_result_t jb_storage_kv_reads(char const* const key_ptr, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len) {
	return jb_storage_kv_read((uint8_t*)key_ptr, strlen(key_ptr), out_ptr, value_offset, value_max_len, out_value_len);
}

jb_result_t jb_storage_kv_read_of(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len) {
	if (out_value_len)
		*out_value_len = 0;

	uint64_t result = jb_host_read(service_id, key_ptr, key_len, out_ptr, value_offset, value_max_len);
	if (result == HOST_NONE) {
		// Key does not exist.
		return JB_ERROR_STORAGE_KV_READ;
	}
	
	if (out_value_len)
		*out_value_len = result;

	return JB_OK;
}

jb_result_t jb_storage_kv_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len, uint64_t* out_old_value_len) {
	if (out_old_value_len)
		*out_old_value_len = 0;

	uint64_t result = jb_host_write(key_ptr, key_len, value_ptr, value_len);
	if (result == HOST_FULL)
		return JB_ERR_INSUFFICIENT_BALANCE;		
	else if (result == HOST_NONE) {
		// No-Op
	} else if (out_old_value_len)
		*out_old_value_len = result;

	return JB_OK;
}

jb_result_t jb_storage_kv_writes(char const* const key_ptr, char const* const value_ptr) {
	return jb_storage_kv_write((uint8_t*)key_ptr, strlen(key_ptr), (uint8_t*)value_ptr, strlen(value_ptr), NULL);
}

jb_result_t jb_storage_kv_delete_existings(char const* const key_ptr) {
	uint64_t res = jb_host_write((uint8_t*)key_ptr, strlen(key_ptr), NULL, 0);

	if (res == HOST_FULL)
		return JB_ERR_INSUFFICIENT_BALANCE; // Unreachable
	else if (res == HOST_NONE)
		return JB_ERR_NO_KEY;
	else
		return JB_OK;
}

jb_result_t jb_storage_kv_deletes(char const* const key_ptr) {
	jb_result_t res = jb_storage_kv_delete_existings(key_ptr);

	if (res == JB_ERR_NO_KEY)
		return JB_OK;

	return res;
}
