#include "taskmaster/options.hpp"
#include "taskmaster/taskmaster.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/signal.hpp"
#include "common/system/fork.hpp"
#include "common/system/setsid.hpp"
#include "common/system/dup2.hpp"

#include "taskmaster/time/timer.hpp"


// -- T A S K M A S T E R -----------------------------------------------------

// -- private static methods --------------------------------------------------

/* shared */
auto sm::taskmaster::_shared(const sm::options& opts) -> self& {
	static self instance = self::_launch(opts);
	return instance;
}

/* launch */
auto sm::taskmaster::_launch(const sm::options& opts) -> self {

	if (opts.daemon() == true) {

		// check for root
		if (::getuid() != 0)
			throw sm::system_error("daemonize: must be root");

		// fork
		auto pid = sm::fork();

		// parent process
		if (pid > 0) {
			opts.~options();
			_exit(0);
		}

		// create new session
		sm::setsid();

		// fork
		pid = sm::fork();

		// second parent process
		if (pid > 0) {
			opts.~options();
			_exit(0);
		}

		// dup
		//sm::dup2(caca, pipi);

		// close standard descriptors
		static_cast<void>(::close(STDIN_FILENO));
		static_cast<void>(::close(STDOUT_FILENO));
		static_cast<void>(::close(STDERR_FILENO));

		// change directory
		static_cast<void>(::chdir("/"));

		// change umask
		::umask(0);

		// setup logger to print into daemon log file
		sm::logger::log_to_daemon();
	}

	// return instance
	return self{opts};
}


// -- private lifecycle -------------------------------------------------------


/* run */
auto sm::taskmaster::_run(void) -> void {

	sm::timer tm{2000, 0};

	_monitor.subscribe(tm, sm::event{EPOLLIN});

	// poll
	while (_running == true) {

		// wait for events
		_monitor.wait(*this);
	}
}

/* default constructor */
sm::taskmaster::taskmaster(const sm::options& opts)
: _runlock{},
  _running{true},
  _server{::in_port_t{4242}},
  _monitor{},
  _clients{} {

	// subscribe to server
	_monitor.subscribe(_server, sm::event{EPOLLIN});

	// subscribe to signals
	_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});
}


// -- public static methods ---------------------------------------------------

/* run */
auto sm::taskmaster::run(const sm::options& opts) -> void {
	self::_shared(opts)._run();
}

/* server */
auto sm::taskmaster::server(void) noexcept -> sm::server& {
	return _server;
}

/* monitor */
auto sm::taskmaster::monitor(void) noexcept -> sm::monitor& {
	return _monitor;
}

/* stop */
auto sm::taskmaster::stop(void) noexcept -> void {
	_running = false;
}

/* clients */
auto sm::taskmaster::clients(void) noexcept -> sm::client_manager& {
	return _clients;
}
