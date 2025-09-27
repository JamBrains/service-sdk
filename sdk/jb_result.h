#pragma once

/// @brief The result of a read host call.
/// @details Indicates failure by returning `JB_READ_RESULT_ERROR` and the out length otherwise.
typedef enum {
	JB_OK = 0,
	JB_ERR_STORAGE_KV_READ = 1,
	JB_ERR_STORAGE_KV_WRITE = 2,
	JB_ERR_INSUFFICIENT_BALANCE = 3,
	JB_ERR_STORAGE_KV_DELETE = 4,
	JB_ERR_NO_KEY = 5,
	JB_ERR_NO_DATA = 6,
	JB_ERR_NO_OUT = 7,
	JB_ERR_UNREACHABLE = 8,
	JB_ERR_UNDECODABLE = 9,
	JB_ERR_TOO_BIG = 10,
	JB_ERR_TRAILING_DATA = 11,
} jb_result_t;

char const* jb_result_name(jb_result_t result);
