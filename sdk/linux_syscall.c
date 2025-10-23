#include "host.h"
#include "host_functions_untyped.h"
#include "jb_log.h"

#include <string.h>
#include <errno.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <stdio.h>

// Porting the stuff from here https://github.com/paritytech/polkaports/blob/3a4ac02d1ffab65636aa903154834a9ed652e24e/crates/polkakernel/src/libc.rs
// (more or less)

const uint64_t THREAD_ID = 1;
const uint64_t TIOCGWINSZ = 0x5413;
const uint64_t IOV_MAX = 1024;

const uint64_t FD_COUT = 1;
const uint64_t FD_CERR = 2;

char const* log_target = "";

// This comes from musl and should pretend to do a Linux syscall.
extern uint64_t pvm_syscall(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6)
{
	switch (syscall_id) {
	case SYS_ioctl: {
		uint64_t fd = arg1;
		uint64_t op = arg2;
		uint64_t address = arg3;

		// TODO check file pointer
		if (op == TIOCGWINSZ) {
			// 80 x 24, xpixel 0, ypixel 0
			uint16_t* winsize = (uint16_t*)address;
			winsize[0] = 80;
			winsize[1] = 24;
			winsize[2] = 0;
			winsize[3] = 0;

			return 0;
		}

		return -ENOSYS;
	}
	case SYS_writev: {
		uint64_t fd = arg1;
		uint64_t* iov = (uint64_t*)arg2;
		uint64_t iovcnt = arg3;

		if (iovcnt == 0 || iovcnt > IOV_MAX) {
			return -EINVAL;
		}

		uint64_t cum_len = 0;
		for (uint64_t i = 0; i < iovcnt; i++) {
			uint64_t address = iov[i * 2];
			uint64_t len = iov[i * 2 + 1];

			if (fd != FD_COUT && fd != FD_CERR) {
				return -EBADF;
			}
			char const* const msg = (char*)(address);
			
			// Skip trailing newlines since we already do them anyway
			if (len > 1 && msg[len - 1] == '\n') {
				len--;
				cum_len++;
			}
			// Also skip if last message in the batch is a newline
			if (i == iovcnt - 1 && len == 1 && msg[0] == '\n') {
				cum_len += len;
				continue;
			}
			if (len == 0) {
				continue;
			}

			uint64_t level = fd == FD_COUT ? JB_LOG_LEVEL_INFO : JB_LOG_LEVEL_ERROR;
			
			jb_host_log_untyped(level, (uint64_t)log_target, strlen(log_target), (uint64_t)msg, len);

			cum_len += len;
		}

		return cum_len;
	}
	case SYS_set_tid_address: {
		*(uint64_t*)arg1 = THREAD_ID;

		return THREAD_ID;
	}
	case SYS_exit_group:
	{
		POLKAVM_TRAP();
		return 0;
	}
	case SYS_exit:
	{
		POLKAVM_TRAP();
		return 0;
	}
	case SYS_clock_gettime:
	{
		return 0;
	}
	default: {
		char buf[100];
		snprintf(buf, sizeof(buf), "unknown syscall %ld", syscall_id);
		jb_log_warn("linux_syscall", buf);
		return -1;
	}
	}

	return 0;
}
