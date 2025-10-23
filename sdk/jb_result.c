#include "jb_result.h"

#define CNT_OF(x) (sizeof(x) / sizeof((x)[0]))

char const* const jb_result_strings[] = {
	"OK",
	"ERROR_STORAGE_KV_READ",
	"ERROR_STORAGE_KV_WRITE",
	"ERR_INSUFFICIENT_BALANCE",
	"ERR_STORAGE_KV_DELETE",
	"JB_ERR_NO_KEY",
	"JB_ERR_NO_DATA",
	"JB_ERR_NO_OUT",
	"JB_ERR_UNREACHABLE",
	"JB_ERR_UNDECODABLE",
	"JB_ERR_TOO_BIG",
	"JB_ERR_TRAILING_DATA",
	"JB_ERR_MALLOC",
};

char const* jb_result_name(jb_result_t result) {
	if (result >= CNT_OF(jb_result_strings)) {
		return "RESULT_UNKNOWN";
	}
	return jb_result_strings[result];
}
