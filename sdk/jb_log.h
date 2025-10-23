#pragma once

#include <stdint.h>

typedef uint64_t jb_log_level_t;

#define JB_LOG_LEVEL_ERROR 0
#define JB_LOG_LEVEL_WARN 1
#define JB_LOG_LEVEL_INFO 2
#define JB_LOG_LEVEL_DEBUG 3
#define JB_LOG_LEVEL_TRACE 4

void jb_log(jb_log_level_t level, char const *const target, char const *const msg);
void jb_log_info(char const *const target, char const *const msg);
void jb_log_warn(char const *const target, char const *const msg);
void jb_log_error(char const *const target, char const *const msg);
