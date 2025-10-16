#include "jb_asset.h"
#include "jb_log.h"
#include "jb_result.h"
#include "host.h"


void assertion_failure(char const *const error_msg)
{
	while (1)
	{
		jb_log_error("JB_ASSERT", error_msg);
		POLKAVM_TRAP();
		// *should* be unreachable, but just in case we put a loop
	}
}

void jb_assert(uint64_t v, char const *const error_msg)
{
	jb_assert_not_zero(v, error_msg);
}

void jb_assert_ok(jb_result_t r, char const *const error_msg)
{
	if (r != JB_OK)
	{
		assertion_failure(error_msg);
	}
}

void jb_assert_not_zero(uint64_t v, char const *const error_msg)
{
	if (v == 0)
	{
		assertion_failure(error_msg);
	}
}

void jb_assert_zero(uint64_t v, char const *const error_msg)
{
	if (v != 0)
	{
		assertion_failure(error_msg);
	}
}

void jb_assert_equal(uint64_t v1, uint64_t v2, char const *const error_msg)
{
	if (v1 != v2)
	{
		assertion_failure(error_msg);
	}
}
