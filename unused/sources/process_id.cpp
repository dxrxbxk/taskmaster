#include "process/process_id.hpp"
#include "diagnostics/exception.hpp"
#include <iostream>


// -- P R O C E S S  I D  -----------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::process_id::process_id(void) noexcept
: _pid{0} {
}

/* pid constructor */
sm::process_id::process_id(const ::pid_t& pid) noexcept
: _pid{pid} {
}


// -- public conversion operators ---------------------------------------------

/* pid_t conversion operator */
sm::process_id::operator const ::pid_t&(void) const noexcept {
	return _pid;
}


// -- public methods ----------------------------------------------------------

/* open */
auto sm::process_id::open(void) const -> sm::unique_fd {

	// open pidfd with syscall
	return sm::unique_fd{static_cast<int>(
				sm::syscall(SYS_pidfd_open, _pid, 0))
	};
}

/* wait */
auto sm::process_id::wait(const int& options) const -> sm::wait_status {

	int status;

	std::cout << "pid: " << _pid << std::endl;

	const auto state = ::waitpid(_pid, &status, options);

	if (state == -1) {
		if (errno == ECHILD)
			return sm::wait_status{0};
		throw sm::system_error("waitpid");
	}

	// WNOHANG
	// return immediately if no child has exited.

	// WUNTRACED
	// also return if a child has stopped (but not traced via ptrace(2)).
	// Status for traced children which have stopped is provided even if this option is not specified.

	// WCONTINUED (since Linux 2.6.10)
	// also return if a stopped child has been resumed by delivery of SIGCONT.


	// < -1
	// meaning wait for any child process whose process group ID is equal to the absolute value of pid.

	// -1
	// meaning wait for any child process.

	// 0
	// meaning wait for any child process whose process group ID is equal to that of the calling process at the time of the call to waitpid().

	// > 0
	// meaning wait for the child whose process ID is equal to the value of pid.


	// waitpid():
	// on success, returns the process ID of the child whose state has changed;
	// if WNOHANG was specified and one or more child(ren) specified by pid exist, but
	// have not yet changed state, then 0 is returned.  On failure, -1 is returned.

	return sm::wait_status{status};
}

/* kill */
auto sm::process_id::kill(const int& signal) const -> void {

	if (::kill(_pid, signal) == -1)
		throw sm::system_error("kill");
}


