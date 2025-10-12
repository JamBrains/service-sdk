#include "jb_storage.h"
#include "host_functions.h"

#include <string.h>
#include <stdio.h>

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
		return JB_ERR_STORAGE_KV_READ;
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
