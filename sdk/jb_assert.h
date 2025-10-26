#pragma once

#include "jb_result.h"

#include <stdint.h>

/// @brief Convenience for `jb_assert_not_zero`.
/// @param v The value to check.
/// @param error_msg The error message to print if the value is zero.
void jb_assert(uint64_t v, char const *const error_msg);

/// @brief Panic if the result is not JB_OK.
/// @param r The result to check.
/// @param error_msg The error message to print if the result is not JB_OK.
void jb_assert_ok(jb_result_t r, char const *const error_msg);

/// @brief Panic if the value is zero.
/// @param v The value to check.
/// @param error_msg The error message to print if the value is zero.
void jb_assert_not_zero(uint64_t v, char const *const error_msg);

/// @brief Panic if the value is not zero.
/// @param v The value to check.
/// @param error_msg The error message to print if the value is not zero.
void jb_assert_zero(uint64_t v, char const *const error_msg);

/// @brief Panic if the values are not equal.
/// @param v1 The first value to check.
/// @param v2 The second value to check.
/// @param error_msg The error message to print if the values are not equal.
void jb_assert_equal(uint64_t v1, uint64_t v2, char const *const error_msg);
