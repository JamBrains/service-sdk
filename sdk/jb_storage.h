#pragma once

#include "jb_service_types.h"

#include <stdint.h>
#include <stdbool.h>

/// @brief Read the value of a key from a service.
/// @param service_id Id of the service to read from.
/// @param key_ptr Pointer to the key to read.
/// @param key_len Length of the key.
/// @param out_ptr Pointer to the output value buffer.
/// @param value_offset Offset into the value.
/// @param value_max_len Maximum length of the output value buffer.
/// @param out_value_len Actual full length of the value. May be NULL.
/// @return True iff the read was successful, false otherwise. The only way this fail is if the key does not exist.
jb_result_t jb_storage_kv_read(uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len);

jb_result_t jb_storage_kv_reads(char const* const key_ptr, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len);

/// @brief Read the value of a key from a service.
/// @param service_id Id of the service to read from.
/// @param key_ptr Pointer to the key to read.
/// @param key_len Length of the key.
/// @param out_ptr Pointer to the output value buffer.
/// @param value_offset Offset into the value.
/// @param value_max_len Maximum length of the output value buffer.
/// @param out_value_len Actual full length of the value. May be NULL.
/// @return True iff the read was successful, false otherwise. The only way this fail is if the key does not exist.
jb_result_t jb_storage_kv_read_of(uint64_t service_id, uint8_t const* const key_ptr, uint64_t key_len, uint8_t* out_ptr, uint64_t value_offset, uint64_t value_max_len, uint64_t* out_value_len);

/// @brief Write a value at a key for a service.
/// @param service_id Id of the service to write to.
/// @param key_ptr Pointer to the key to write to.
/// @param key_len Length of the key.
/// @param value_ptr Pointer to the value to write.
/// @param value_len Length of the value.
/// @param out_old_value_len Length of the old value. May be NULL.
jb_result_t jb_storage_kv_write(uint8_t const* const key_ptr, uint64_t key_len, uint8_t const* const value_ptr, uint64_t value_len, uint64_t* out_old_value_len);

jb_result_t jb_storage_kv_writes(char const* const key_ptr, char const* const value_ptr);

jb_result_t jb_storage_kv_delete_existings(char const* const key_ptr);
jb_result_t jb_storage_kv_deletes(char const* const key_ptr);

void assert_ok(jb_result_t);
