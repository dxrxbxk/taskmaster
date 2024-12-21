#include "taskmaster.hpp"
#include "core/process.hpp"
#include "core/profile.hpp"
#include "log/logger.hpp"
#include "pipe_exec.hpp"

#include "system/fork.hpp"
#include "system/access.hpp"
#include "system/chdir.hpp"
#include "system/dup2.hpp"
#include "system/execve.hpp"
#include "system/open.hpp"
#include "system/syscall.hpp"

#include <sys/syscall.h>

#include <sys/wait.h>
#include <string_view>


// -- P R O C E S S -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* profile constructor */
sm::process::process(const sm::shared_ptr<sm::profile>& profile)
: _profile{profile},
  _state{S_STOPPED},
  _pid{0},
  _pidfd{},
  _starttimer{},
  _stoptimer{} {
}

/* move constructor */
sm::process::process(self&& other) noexcept
: _profile{std::move(other._profile)},
  _state{other._state},
  _pid{other._pid},
  _pidfd{std::move(other._pidfd)} {

	// invalidate other
	_state = S_STOPPED;
	_pid   = 0;
}

/* destructor */
sm::process::~process(void) noexcept {

	// check if process is running
	if (_pid == 0)
		return;

	// send signal
	static_cast<void>(::kill(_pid, SIGKILL));
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::process::fd(void) const noexcept -> int {
	return _pidfd;
}

/* on_event */
auto sm::process::on_event(const sm::event& events, sm::taskmaster& tm) -> void {


	// check type of event
	if (not events.is_in())
		throw sm::runtime_error("process: unexpected event");



	::siginfo_t info;
	const auto status = ::waitid(P_PIDFD, (__id_t)_pidfd, &info, WEXITED);

	// check for error
	if (status == -1) {

		// throw for fatal error
		if (errno != ECHILD)
			throw sm::system_error("waitid");

		sm::logger::warn("process: already collected or does not exist.");
		self::disconnect(tm);
		return;
	}

	switch (info.si_code) {

		case CLD_EXITED:
			sm::logger::info("process: exited normally");
			break;

		case CLD_KILLED:
			sm::logger::info("process: was killed by signal [", info.si_status, "]");
			break;

		case CLD_DUMPED:
			sm::logger::info("process: terminated abnormally");
			break;

		case CLD_TRAPPED:
			sm::logger::info("process: traced child has trapped");
			break;

		case CLD_STOPPED:
			sm::logger::info("process: child has stopped");
			break;

		default:
			sm::logger::info("process: unknown exit code");
			break;
	}

	tm.readline().prompt();

	// disconnect
	self::disconnect(tm);
}


// -- public methods ----------------------------------------------------------

/* start */
auto sm::process::start(sm::taskmaster& tm) -> void {

	if (_state == S_STARTING) {
		sm::logger::warn("program already starting");
		return;
	}
	else if (_state == S_RUNNING) {
		sm::logger::warn("program already running");
		return;
	}

	//if (_is_starting) {
	//	sm::logger::warn("program already starting");
	//	return;
	//}

	// check if program is running
	if (_pid != 0) {
		sm::logger::warn("program already running");
		return;
	}


	// create pipe
	sm::pipe_exec pipe;

	// fork process
	_pid = sm::fork();

	// check for child
	if (_pid == 0) {

		try {

			auto& cmd = _profile->cmd();
			auto& env = _profile->env();

			// test if program is executable
			sm::access<X_OK>(cmd[0U]);

			// chdir
			sm::chdir(_profile->workingdir().data());


			// set umask
			//throw sm::runtime_error("test", "to", "simulate", "an", "error", "in", "the", "program", "execution");

			// close stdin
			//static_cast<void>(::close(STDIN_FILENO));

			// redirect standard descriptors
			sm::unique_fd fds[] {
				sm::open(_profile->stdout().data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644),
				sm::open(_profile->stdout().data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644)
			};

			// print process group id
			sm::logger::info("process group id: ", ::getpgrp());

			sm::dup2(fds[0U], STDOUT_FILENO);
			sm::dup2(fds[1U], STDERR_FILENO);


			// execute program
			sm::execve(cmd[0U], cmd.data(), env.data());

		} catch (const sm::exception& e) {

			// write to pipe
			pipe.write(e.what());

			//_exit(EXIT_FAILURE);
			throw sm::exit{EXIT_FAILURE};
		}

	}


	// -- parent process ------------------------------------------------------

	// check for error
	if (pipe.read() != 0U) {

		// log error
		sm::logger::error(std::string_view{pipe.data()});

		// wait for child
		::waitpid(_pid, nullptr, 0);

		// reset pid
		_pid = 0;
	}
	else {

		// get monitor
		auto& monitor = tm.monitor();

		// launch start timer
		_starttimer = sm::timer{*this,
			&sm::process::start_event,
			_profile->starttime() * 1000U};

		monitor.subscribe(_starttimer, sm::event{EPOLLIN});

		_state = S_STARTING;
		//_is_starting = true;

		sm::logger::info(std::string_view{_profile->cmd()[0U]});

		_pidfd = static_cast<int>(sm::syscall(SYS_pidfd_open, _pid, 0));

		monitor.subscribe(*this, sm::event{EPOLLIN});
	}
}

/* stop */
auto sm::process::stop(sm::taskmaster& tm) -> void {

	// check if program is running
	if (_pid == 0)
		return sm::logger::warn("process: not running");

	if (_state == S_STOPPING)
		return sm::logger::warn("process: already stopping");

	_state = S_STOPPING;

	if (_profile->stoptime() == 0U) {
		
		// send signal (SIGKILL)
		if (::kill(_pid, SIGKILL) == -1)
			throw sm::system_error("kill");

		return;
	}

	// send signal
	if (::kill(_pid, _profile->stopsignal()) == -1)
		throw sm::system_error("kill");

	// get monitor
	auto& monitor = tm.monitor();

	// launch stop timer
	_stoptimer = sm::timer{*this,
		&sm::process::stop_event,
		_profile->stoptime() * 1000U};

	monitor.subscribe(_stoptimer, sm::event{EPOLLIN});

}

/* restart */
auto sm::process::restart(sm::taskmaster& tm) -> void {
}

/* disconnect */
auto sm::process::disconnect(sm::taskmaster& tm) -> void {

	// get monitor
	auto& monitor = tm.monitor();

	// remove timers, pidfd
	monitor.unsubscribe(_starttimer, _stoptimer, *this);

	// close pidfd
	_pidfd.close();

	// reset pid
	_pid = 0;

	// reset state
	_state = S_STOPPED;
}


/* status */
auto sm::process::status(void) const -> void {

	// get command
	const std::string_view cmd{_profile->cmd()[0U]};

	switch (_state) {

		case S_STOPPED:
			sm::logger::info("process: ", cmd, " [STOPPED]");
			break;

		case S_STARTING:
			sm::logger::info("process: ", cmd, " [STARTING]");
			break;

		case S_RUNNING:
			sm::logger::info("process: ", cmd, " [RUNNING]");
			break;

		case S_BACKOFF:
			sm::logger::info("process: ", cmd, " [BACKOFF]");
			break;

		case S_STOPPING:
			sm::logger::info("process: ", cmd, " [STOPPING]");
			break;

		case S_EXITED:
			sm::logger::info("process: ", cmd, " [EXITED]");

		case S_FATAL:
			sm::logger::info("process: ", cmd, " [FATAL]");
			break;

		default:
			sm::logger::info("process: ", cmd, " [UNKNOWN]");
			break;
	}
}


// -- public callbacks --------------------------------------------------------

/* start event */
auto sm::process::start_event(sm::taskmaster& tm) -> void {

	// log message
	sm::logger::info("program: ",
					 std::string_view{_profile->cmd()[0U]},
					 " successfully launched [",
					 _pid, "]");

	tm.readline().prompt();

	// remove timer
	tm.monitor().unsubscribe(_starttimer);

	// reset flag
	_state = S_RUNNING;
}

/* stop event */
auto sm::process::stop_event(sm::taskmaster& tm) -> void {

	if (_pid == 0) {
		sm::logger::warn("program: ",
						 std::string_view{_profile->cmd()[0U]},
						 " gracefully stopped");

		tm.readline().prompt();
		return;
	}

	// send signal
	if (::kill(_pid, SIGKILL) == -1)
		throw sm::system_error("kill");

	// log message
	sm::logger::warn("program: ",
					 std::string_view{_profile->cmd()[0U]},
					 " forcefully stopped (killed)");

	// remove timer
	tm.monitor().unsubscribe(_stoptimer);

	tm.readline().prompt();

	// reset flag
	_state = S_STOPPED;
}

// -- public accessors --------------------------------------------------------

/* is running */
auto sm::process::is_running(void) const noexcept -> bool {
	return _pid != 0;
}

/* pid */
auto sm::process::pid(void) const noexcept -> const ::pid_t& {
	return _pid;
}






