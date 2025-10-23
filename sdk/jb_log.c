#include "jb_log.h"
#include "host_functions.h"
#include <string.h>

void jb_log(jb_log_level_t level, char const *const target, char const *const msg)
{
	jb_host_log(level, (uint8_t*)target, strlen(target), (uint8_t*)msg, strlen(msg));
}

void jb_log_info(char const *const target, char const *const msg)
{
	jb_log(JB_LOG_LEVEL_INFO, target, msg);
}

void jb_log_warn(char const *const target, char const *const msg)
{
	jb_log(JB_LOG_LEVEL_WARN, target, msg);
}

void jb_log_error(char const *const target, char const *const msg)
{
	jb_log(JB_LOG_LEVEL_ERROR, target, msg);
}
