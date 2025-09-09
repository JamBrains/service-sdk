#include "jb_service_types.h"

#define CNT_OF(x) (sizeof(x) / sizeof((x)[0]))

char const* const jb_result_strings[] = {
	"OK",
	"ERROR_STORAGE_KV_READ",
	"ERROR_STORAGE_KV_WRITE",
	"ERR_INSUFFICIENT_BALANCE",
	"ERR_STORAGE_KV_DELETE",
	"JB_ERR_NO_KEY"
};

char const* jb_result_name(jb_result_t result) {
	if (result >= CNT_OF(jb_result_strings)) {
		return "RESULT_UNKNOWN";
	}
	return jb_result_strings[result];
}
