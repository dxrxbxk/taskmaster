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
  _numprocs{sm::core_affinity::max_cores()},
  _umask{0U},
  _workingdir{"/"},
  _autostart{false},
  _autorestart{FALSE},
  _exitcodes{},
  _startretries{0U},
  _starttime{0U},
  _stopsignal{0},
  _stoptime{0U},
  _stdout{"/dev/null"},
  _stderr{"/dev/null"},
  _env{} {
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

			// become group leader
			//if (::setpgid(0, 0) == -1)
			//	throw sm::system_error("setpgid");

			// test if program is executable
			sm::access<X_OK>(_cmd[0U]);

			// chdir
			sm::chdir(_workingdir.data());

			// cpu affinity
			sm::core_affinity::set(_numprocs);

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
		sm::logger::error(pipe.data());

		// wait for child
		::waitpid(_pid, nullptr, 0);

		// reset pid
		_pid = 0;
	}
	else {

		sm::logger::info(_cmd[0U]);

		// get monitor
		auto& monitor = tm.monitor();
		_pidfd = static_cast<int>(sm::syscall(SYS_pidfd_open, _pid, 0));

		monitor.subscribe(*this, sm::event{EPOLLIN});
	}
}

/* stop */
auto sm::program::stop(void) -> void {

	// check if program is running
	if (_pid == 0) {
		sm::logger::warn("program not running");
		return;
	}

	// send signal
	//if (::kill(_pid, _stopsignal) == -1)
	//	throw sm::system_error("kill");
	if (::kill(_pid, SIGTERM) == -1)
		throw sm::system_error("kill");


	// reset pid
	_pid = 0;
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

	// unsubscribe
	tm.monitor().unsubscribe(*this);

	// close pidfd
	_pidfd.close();

	// reset pid
	_pid = 0;
}
