#include "taskmaster/options.hpp"
#include "taskmaster/taskmaster.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/signal.hpp"
#include "common/system/fork.hpp"
#include "common/system/chdir.hpp"
#include "common/system/setsid.hpp"
#include "common/system/dup2.hpp"

#include "taskmaster/program.hpp"

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
			throw sm::runtime_error("taskmaster: must be root");


		sm::logger::info("------------------------------------------------------------");
		sm::logger::warn("taskmaster: is launching as daemon");
		sm::logger::info("taskmaster: you can find logs in \x1b[32m/var/log/taskmaster.log\x1b[0m");
		sm::logger::info("taskmaster: enjoy!");
		sm::logger::info("------------------------------------------------------------");



		// fork
		auto pid = sm::fork();

		// parent process
		if (pid > 0) {
			throw sm::exit{EXIT_SUCCESS};
		}

		// create new session
		sm::setsid();

		// fork
		pid = sm::fork();

		// second parent process
		if (pid > 0) {
			throw sm::exit{EXIT_SUCCESS};
			//opts.~options();
			//_exit(0);
		}

		// close standard descriptors
		static_cast<void>(::close(STDIN_FILENO));
		static_cast<void>(::close(STDOUT_FILENO));
		static_cast<void>(::close(STDERR_FILENO));

		// change directory
		sm::chdir("/");

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

	//sm::timer tm{3'000, 300};
	//_monitor.subscribe(tm, sm::event{EPOLLIN});

	sm::program prog{"test"};

	prog.cmd_push("/root/data/taskmaster/hello_taskmaster");

	prog.stdout("/root/data/taskmaster/ls.out");
	prog.stderr("/root/data/taskmaster/ls.err");

	prog.execute(*this);


	// poll
	while (_running == true) {

		// wait for events
		_monitor.wait(*this);
	}
}


static auto pid(void) -> ::pid_t {
	static ::pid_t pid = ::getpid();
	return pid;
}

/* default constructor */
sm::taskmaster::taskmaster(const sm::options& opts)
: _runlock{},
  _config{},
  _running{true},
  _server{opts.port()},
  _monitor{},
  _clients{},
  _programs{} {


	// set config from options
	if (opts.has_config() == true)
		_config.path(opts.config());

	// parse config
	_config.parse(_programs);

	pid();

	// subscribe to server
	_monitor.subscribe(_server, sm::event{EPOLLIN});

	// subscribe to signals
	_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});
}

/* destructor */
sm::taskmaster::~taskmaster(void) noexcept {

	// avoid double destruction
	if (pid() != ::getpid())
		return;

	sm::logger::warn("taskmaster: stopping");
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
