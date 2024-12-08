#include "common/signal.hpp"
#include "taskmaster/controller.hpp"
#include <stdexcept>
#include <unistd.h>
#include <sys/mman.h>


// -- S I G N A L -------------------------------------------------------------

// -- private static methods --------------------------------------------------

/* signal handler */
auto sm::signal::_handler(const int sig) noexcept -> void {

	// write signal to pipe
	static_cast<void>(
		::write(self::shared()._pipe[1U], &sig, sizeof(sig))
	);
}

/* record */
auto sm::signal::_record(const int& sig) -> void {

	if (::signal(sig, self::_handler) == SIG_ERR)
		throw std::runtime_error("Failed to record signal");
}


// -- private lifecycle -------------------------------------------------------

/* default constructor */
sm::signal::signal(void)
: _pipe{} {

	int* ptr = reinterpret_cast<int*>(_pipe);

	// create pipe
	if (::pipe(ptr) == -1)
		throw std::runtime_error("Failed to create pipe");


	self::_record<
		// interactive attention signal
		SIGINT,
		// abnormal termination
		SIGABRT,
		// erroneous arithmetic operation
		SIGFPE,
		// invalid access to storage
		SIGSEGV,
		// termination request
		SIGTERM,
		// hangup
		SIGHUP,
		// quit
		SIGQUIT,
		// trace/breakpoint trap
		SIGTRAP,
		// killed
		//SIGKILL,
		// broken pipe
		SIGPIPE,
		// alarm clock
		SIGALRM
			>();
}

/*
SIGABRT      P1990      Core    Abort signal from abort(3)
SIGALRM      P1990      Term    Timer signal from alarm(2)
SIGBUS       P2001      Core    Bus error (bad memory access)
SIGCHLD      P1990      Ign     Child stopped or terminated
SIGCLD         -        Ign     A synonym for SIGCHLD
SIGCONT      P1990      Cont    Continue if stopped
SIGEMT         -        Term    Emulator trap
SIGFPE       P1990      Core    Floating-point exception
SIGHUP       P1990      Term    Hangup detected on controlling terminal
								or death of controlling process
SIGILL       P1990      Core    Illegal Instruction
SIGINFO        -                A synonym for SIGPWR
SIGINT       P1990      Term    Interrupt from keyboard
SIGIO          -        Term    I/O now possible (4.2BSD)
SIGIOT         -        Core    IOT trap. A synonym for SIGABRT
SIGKILL      P1990      Term    Kill signal
SIGLOST        -        Term    File lock lost (unused)
SIGPIPE      P1990      Term    Broken pipe: write to pipe with no
								readers; see pipe(7)
SIGPOLL      P2001      Term    Pollable event (Sys V);
								synonym for SIGIO
SIGPROF      P2001      Term    Profiling timer expired
SIGPWR         -        Term    Power failure (System V)
SIGQUIT      P1990      Core    Quit from keyboard
SIGSEGV      P1990      Core    Invalid memory reference
SIGSTKFLT      -        Term    Stack fault on coprocessor (unused)
SIGSTOP      P1990      Stop    Stop process
SIGTSTP      P1990      Stop    Stop typed at terminal
SIGSYS       P2001      Core    Bad system call (SVr4);
								see also seccomp(2)
SIGTERM      P1990      Term    Termination signal
SIGTRAP      P2001      Core    Trace/breakpoint trap
SIGTTIN      P1990      Stop    Terminal input for background process
SIGTTOU      P1990      Stop    Terminal output for background process
SIGUNUSED      -        Core    Synonymous with SIGSYS
SIGURG       P2001      Ign     Urgent condition on socket (4.2BSD)
SIGUSR1      P1990      Term    User-defined signal 1
SIGUSR2      P1990      Term    User-defined signal 2
SIGVTALRM    P2001      Term    Virtual alarm clock (4.2BSD)
SIGXCPU      P2001      Core    CPU time limit exceeded (4.2BSD);
								see setrlimit(2)
SIGXFSZ      P2001      Core    File size limit exceeded (4.2BSD);
								see setrlimit(2)
SIGWINCH       -        Ign     Window resize signal (4.3BSD, Sun)
*/


// -- public static methods ---------------------------------------------------

/* shared */
auto sm::signal::shared(void) -> self& {
	static self instance;
	return instance;
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::signal::fd(void) const noexcept -> int {
	return _pipe[0U];
}


/* on event */
auto sm::signal::on_event(sm::controller& control, const sm::event& events) -> void {

	if (events.is_in() != true)
		return;

	int sig;

	if (::read(fd(), &sig, sizeof(sig)) == -1)
		throw std::runtime_error("failed to read signal");

	switch (sig) {

		case SIGINT:
			::write(STDOUT_FILENO, "SIGINT\n", 7);
			break;
		case SIGILL:
			::write(STDOUT_FILENO, "SIGILL\n", 7);
			break;
		case SIGSTOP:
			::write(STDOUT_FILENO, "SIGSTOP\n", 8);
			break;
		case SIGABRT:
			::write(STDOUT_FILENO, "SIGABRT\n", 8);
			break;
		case SIGFPE:
			::write(STDOUT_FILENO, "SIGFPE\n", 7);
			break;
		case SIGSEGV:
			::write(STDOUT_FILENO, "SIGSEGV\n", 8);
			break;
		case SIGTERM:
			::write(STDOUT_FILENO, "SIGTERM\n", 8);
			break;
		case SIGHUP:
			::write(STDOUT_FILENO, "SIGHUP\n", 7);
			break;
		case SIGQUIT:
			::write(STDOUT_FILENO, "SIGQUIT\n", 8);
			break;
		case SIGTRAP:
			::write(STDOUT_FILENO, "SIGTRAP\n", 8);
			break;
		case SIGPIPE:
			::write(STDOUT_FILENO, "SIGPIPE\n", 8);
			break;
		case SIGALRM:
			::write(STDOUT_FILENO, "SIGALRM\n", 8);
			break;
		default:
			::write(STDOUT_FILENO, "SIGNAL\n", 7);
			break;
	}

	control.stop();
}



