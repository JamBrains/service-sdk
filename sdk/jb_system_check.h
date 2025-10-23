#pragma once

#if !defined(__STDC_VERSION__) || __STDC_VERSION__ < 202311L
#error "This code requires GNU-C23 or later. Compile with -std=gnu23"
#endif

/// @brief Sanity check to ensure that everything works as expected.
void jb_system_check_full();

void jb_system_check_types();
void jb_system_check_fetch();
void jb_system_check_fpu();

void jb_system_check_memory();
void jb_system_check_sbrk();
