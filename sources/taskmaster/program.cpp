#include "taskmaster/program.hpp"
#include "taskmaster/taskmaster.hpp"
#include "taskmaster/log/logger.hpp"

#include "common/system/syscall.hpp"
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

/* execute */
auto sm::program::execute(sm::taskmaster& tm) -> void {

	// check if program is running
	if (_pid != 0)
		return;


	// fork process
	_pid = sm::fork();

	// check for child
	if (_pid == 0) {


		// test if program is executable
		sm::access<X_OK>(_cmd[0U]);

		// close stdin (maybe...)
		static_cast<void>(::close(STDIN_FILENO));

		// chdir
		// cpu affinity
		sm::core_affinity::set(_numprocs);

		// set umask

		// redirect standard descriptors
		sm::unique_fd fds[] {
			sm::open(_stdout.data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644),
			sm::open(_stderr.data(), O_WRONLY | O_APPEND | O_CREAT, /* mode */ 0644)
		};

		sm::dup2(fds[0U], STDOUT_FILENO);
		sm::dup2(fds[1U], STDERR_FILENO);

		// execute program
		sm::execve(_cmd[0U], _cmd.data(), _env.data());
	}

	else { // -- parent process

		sm::logger::info(_cmd[0U]);

		// get monitor
		auto& monitor = tm.monitor();
		_pidfd = static_cast<int>(sm::syscall(SYS_pidfd_open, _pid, 0));

		monitor.subscribe(*this, sm::event{EPOLLIN});

	}
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::program::fd(void) const noexcept -> int {
	return _pidfd;
}

/* on_event */
auto sm::program::on_event(const sm::event& events, sm::taskmaster& tm) -> void {


	// check for error
	if (events & EPOLLERR)
		throw sm::runtime_error("program received epollerr");

	// check for hangup
	if (events & EPOLLHUP) {

		// log hangup
		sm::logger::info("program hangup");
	}

	// check for input
	if (events & EPOLLIN) {

		::siginfo_t info;
		const auto status = ::waitid(P_PIDFD, (__id_t)_pidfd, &info, WEXITED);

		if (status == -1) {

			// check for fatal error
			if (errno != ECHILD)
				throw sm::system_error("waitid");

			sm::logger::info("Process already collected or does not exist.");
			self::disconnect(tm);
			return;
		}

		if (info.si_code == CLD_EXITED) {
			sm::logger::info("Process exited normally");
		} else if (info.si_code == CLD_KILLED || info.si_code == CLD_DUMPED) {
			sm::logger::info("Process was terminated by signal");
		}

		self::disconnect(tm);

		// restart program
		self::execute(tm);
	}

}

auto sm::program::disconnect(sm::taskmaster& tm) -> void {

	// unsubscribe
	tm.monitor().unsubscribe(*this);

	// close pidfd
	_pidfd.close();

	// reset pid
	_pid = 0;
}
