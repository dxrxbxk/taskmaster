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

/* execute */
auto sm::program::execute(sm::taskmaster& tm) -> void {

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

			// cpu affinity
			sm::core_affinity::set(_numprocs);

			// set umask
			throw sm::runtime_error("test", "to", "simulate", "an", "error", "in", "the", "program", "execution");

			// close stdin
			static_cast<void>(::close(STDIN_FILENO));

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
	if (_pid == 0)
		return;

	// send signal
	if (::kill(_pid, _stopsignal) == -1)
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


	// check for error
	if (events.is_err())
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
