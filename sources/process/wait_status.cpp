#include "process/wait_status.hpp"

// -- W A I T  S T A T U S ----------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::wait_status::wait_status(void) noexcept
: _status{0} {
}

/* status constructor */
sm::wait_status::wait_status(const int& status) noexcept
: _status{status} {
}


// -- public accessors --------------------------------------------------------

/* is terminated normally */
auto sm::wait_status::is_terminated_normally(void) const noexcept -> bool {
	// returns true if the child terminated normally,
	// that is, by calling exit(3) or _exit(2), or by returning from main().
	return WIFEXITED(_status);
}

/* exit status */
auto sm::wait_status::exit_status(void) const noexcept -> int {
	// returns the exit status of the child.
	// This consists of the least significant 8 bits of the status argument that the child specified in a call to exit(3)
	// or _exit(2) or as the argument for a return statement in main().
	// This macro should be employed only if WIFEXITED returned true.
	return WEXITSTATUS(_status);
}

/* is signaled */
auto sm::wait_status::is_signaled(void) const noexcept -> bool {
	// returns true if the child process was terminated by a signal.
	return WIFSIGNALED(_status);
}

/* signal */
auto sm::wait_status::signal(void) const noexcept -> int {
	// returns the number of the signal that caused the child process to terminate.
	// This macro should be employed only if WIFSIGNALED returned true.
	return WTERMSIG(_status);
}

/* is core dumped */
auto sm::wait_status::is_core_dumped(void) const noexcept -> bool {
	// returns true if the child produced a core dump (see core(5)).
	// This macro should be employed only if WIFSIGNALED returned true.
	return WCOREDUMP(_status);
}

/* is stopped */
auto sm::wait_status::is_stopped(void) const noexcept -> bool {
	// returns  true if the child process was stopped by delivery of a signal;
	// this is possible only if the call was done using WUNTRACED or when the child is being traced (see ptrace(2)).
	return WIFSTOPPED(_status);
}

/* stop signal */
auto sm::wait_status::stop_signal(void) const noexcept -> int {
	// returns the number of the signal which caused the child to stop.
	// This macro should be employed only if WIFSTOPPED returned true.
	return WSTOPSIG(_status);
}

/* is continued */
auto sm::wait_status::is_continued(void) const noexcept -> bool {
	// (since Linux 2.6.10) returns true if the child process was resumed by delivery of SIGCONT.
	return WIFCONTINUED(_status);
}
