#include "taskmaster.hpp"
#include "signal.hpp"
#include "log/logger.hpp"
#include "diagnostics/exception.hpp"


#include <unistd.h>


#include <iostream>


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

	struct ::sigaction sa;
	sa.sa_handler = self::_handler;
	sa.sa_mask = __sigset_t{};
	sa.sa_flags = SA_RESTART;

	if (::sigaction(sig, &sa, nullptr) == -1)
		throw sm::system_error("sigaction");
}

// -- private methods ---------------------------------------------------------

auto sm::signal::_fill_map(void) -> void {

	_signals["INT"]  = SIGINT;
	_signals["ABRT"] = SIGABRT;
	_signals["FPE"]  = SIGFPE;
	_signals["SEGV"] = SIGSEGV;
	_signals["TERM"] = SIGTERM;
	_signals["HUP"]  = SIGHUP;
	_signals["QUIT"] = SIGQUIT;
	_signals["TRAP"] = SIGTRAP;
	_signals["TSTP"] = SIGTSTP;
	_signals["CONT"] = SIGCONT;
	_signals["PIPE"] = SIGPIPE;
	_signals["ALRM"] = SIGALRM;
	_signals["STOP"] = SIGSTOP;
}


// -- private lifecycle -------------------------------------------------------

/* default constructor */
sm::signal::signal(void)
: _pipe{} {

	_fill_map();
	

	// check for dangerous cast
	static_assert(sizeof(sm::unique_fd) == sizeof(int));

	// create pipe
	if (::pipe(reinterpret_cast<int*>(_pipe)) == -1)
		throw sm::system_error("pipe");


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
		// stop (tty)
		SIGTSTP,
		// continue if stopped
		SIGCONT,
		// broken pipe
		SIGPIPE,
		// alarm clock
		SIGALRM
			>();
}



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

#include <iostream>

/* on event */
auto sm::signal::on_event(const sm::event& events, sm::taskmaster& tm) -> void {

	if (events.is_in() != true)
		return;

	int sig;

	if (::read(fd(), &sig, sizeof(sig)) == -1)
		throw sm::system_error("read");

	switch (sig) {

		case SIGINT:
			sm::logger::signal("SIGINT");
			break;

		case SIGABRT:
			sm::logger::signal("SIGABRT");
			break;

		case SIGFPE:
			sm::logger::signal("SIGFPE");
			break;

		case SIGSEGV:
			sm::logger::signal("SIGSEGV");
			break;

		case SIGTERM:
			sm::logger::signal("SIGTERM");
			break;

		case SIGHUP:
			sm::logger::signal("SIGHUP");
			tm.reload();
			break;

		case SIGQUIT:
			sm::logger::signal("SIGQUIT");
			break;

		case SIGTRAP:
			sm::logger::signal("SIGTRAP");
			break;

		case SIGTSTP:
			sm::logger::signal("SIGTSTP");
			return;

		case SIGCONT:
			sm::logger::signal("SIGCONT");
			return;

		case SIGPIPE:
			sm::logger::signal("SIGPIPE");
			break;

		case SIGALRM:
			sm::logger::signal("SIGALRM");
			break;

		default:
			sm::logger::signal("UNKNOWN");
			break;
	}

	// stop taskmaster
	tm.stop();
}


// -- public methods ----------------------------------------------------------

auto sm::signal::to_int(const std::string& sig) -> int {

	return self::has_signal(sig.data(), sig.size());

	auto it = shared()._signals.find(sig);

	if (it == shared()._signals.end())
		throw sm::runtime_error("invalid signal");

	return it->second;
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
