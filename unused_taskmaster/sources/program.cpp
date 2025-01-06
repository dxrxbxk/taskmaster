#include "program.hpp"
#include "taskmaster.hpp"
#include "log/logger.hpp"
#include "pipe_exec.hpp"

#include "system/syscall.hpp"
#include "system/chdir.hpp"

#include <sys/syscall.h>

#include <signal.h>
#include <sys/wait.h>




// -- P R O G R A M -----------------------------------------------------------

/* id constructor */
sm::program::program(std::string&& id)
: _id{std::move(id)},
  _pid{0},
  _cmd{},
  _numprocs{1U},
  _umask{0777U},
  _autostart{false},
  _autorestart{FALSE},
  _exitcodes{},
  _startretries{0U},
  _starttime{2U},
  _stopsignal{2U},
  _stoptime{0U},
  _workingdir{"/"},
  _stdout{"/dev/null"},
  _stderr{"/dev/null"},
  _env{},
  _starttimer{},
  _is_starting{false},
  _stoptimer{},
  _is_stopping{false} {
}

/* destructor */
sm::program::~program(void) noexcept {


	if (_pid == 0)
		return;

	// send signal
	static_cast<void>(::kill(_pid, SIGKILL));

	// does i send SIGKILL ?
}


// -- public methods ----------------------------------------------------------

/* start */
auto sm::program::start(sm::taskmaster& tm) -> void {

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

			// test if program is executable
			sm::access<X_OK>(_cmd[0U]);

			// chdir
			sm::chdir(_workingdir.data());


			// set umask
			//throw sm::runtime_error("test", "to", "simulate", "an", "error", "in", "the", "program", "execution");

			// close stdin
			//static_cast<void>(::close(STDIN_FILENO));

			// redirect standard descriptors
			sm::unique_fd fds[] {
				sm::open(_stdout.data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644),
				sm::open(_stderr.data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644)
			};

			sm::dup2(fds[0U], STDOUT_FILENO);
			sm::dup2(fds[1U], STDERR_FILENO);

			// execute program
			sm::execve(_cmd[0U], _cmd.data(), _env.data());

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
		//_starttimer = sm::timer{*this,
		//	&sm::program::start_event,
		//	_starttime * 1000U};

		//monitor.subscribe(_starttimer, sm::event{EPOLLIN});

		sm::logger::info(std::string_view{_cmd[0U]});

		_pidfd = static_cast<int>(sm::syscall(SYS_pidfd_open, _pid, 0));

		monitor.subscribe(*this, sm::event{EPOLLIN});
	}
}

/* stop */
auto sm::program::stop(sm::taskmaster& tm) -> void {

	// check if program is running
	if (_pid == 0) {
		sm::logger::warn("program not running");
		return;
	}

	if (_is_stopping) {
		sm::logger::warn("program already stopping");
		return;
	}

	// send signal
	if (::kill(_pid, _stopsignal) == -1)
		throw sm::system_error("kill");

	// get monitor
	auto& monitor = tm.monitor();

	// launch stop timer
	//_stoptimer = sm::timer{*this,
	//	&sm::program::stop_event,
	//	_stoptime * 1000U};
	//
	//monitor.subscribe(_stoptimer, sm::event{EPOLLIN});

	_is_stopping = true;
}

/* start event */
auto sm::program::start_event(sm::taskmaster& tm) -> void {

	// log message
	sm::logger::info("program: ",
					 std::string_view{_cmd[0U]},
					 " successfully launched [",
					 _pid, "]");

	// remove timer
	tm.monitor().unsubscribe(_starttimer);
}

/* stop event */
auto sm::program::stop_event(sm::taskmaster& tm) -> void {

	if (_pid == 0) {
		sm::logger::warn("program: ",
						 std::string_view{_cmd[0U]},
						 " gracefully stopped");
		return;
	}

	// send signal
	if (::kill(_pid, SIGKILL) == -1)
		throw sm::system_error("kill");

	// log message
	sm::logger::warn("program: ",
					 std::string_view{_cmd[0U]},
					 " forcefully stopped (killed)");

	// disconnect
	//self::disconnect(tm);
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::program::fd(void) const noexcept -> int {
	return _pidfd;
}

/* on_event */
auto sm::program::on_event(const sm::event& events, sm::taskmaster& tm) -> void {


	// check type of event
	if (not events.is_in())
		throw sm::runtime_error("program: unexpected event");



	::siginfo_t info;
	const auto status = ::waitid(P_PIDFD, (__id_t)_pidfd, &info, WEXITED);

	// check for error
	if (status == -1) {

		// throw for fatal error
		if (errno != ECHILD)
			throw sm::system_error("waitid");

		sm::logger::warn("Process already collected or does not exist.");
		self::disconnect(tm);
		return;
	}

	switch (info.si_code) {

		case CLD_EXITED:
			sm::logger::info("Process exited normally");
			break;

		case CLD_KILLED:
			sm::logger::info("Process was killed by signal");
			break;

		case CLD_DUMPED:
			sm::logger::info("Process terminated abnormally");
			break;

		case CLD_TRAPPED:
			sm::logger::info("Traced child has trapped");
			break;

		case CLD_STOPPED:
			sm::logger::info("Child has stopped");
			break;

		default:
			sm::logger::info("Unknown exit code");
			break;
	}

	// disconnect
	self::disconnect(tm);

}

auto sm::program::disconnect(sm::taskmaster& tm) -> void {

	// remove timers
	tm.monitor().unsubscribe(_starttimer);
	tm.monitor().unsubscribe(_stoptimer);

	// unsubscribe
	tm.monitor().unsubscribe(*this);

	// close pidfd
	_pidfd.close();

	// reset pid
	_pid = 0;

	// reset timers
	_is_starting = false;
	_is_stopping = false;
}




