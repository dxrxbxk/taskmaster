#include "options.hpp"
#include "taskmaster.hpp"
#include "log/logger.hpp"
#include "signal.hpp"
#include "readline.hpp"

#include "time/timer.hpp"



// -- T A S K M A S T E R -----------------------------------------------------

// -- private static methods --------------------------------------------------

/* shared */
auto sm::taskmaster::_shared(const sm::options& opts) -> self& {
	static self instance{opts};
	return instance;
}


// -- private methods ---------------------------------------------------------

/* run */
auto sm::taskmaster::_run(void) -> void {


	//::pthread_t th{};
	//sm::readline rl{};
	//_monitor.subscribe(rl, sm::event{EPOLLIN});

	sm::readline rl{};
	_monitor.subscribe(rl, sm::event{EPOLLIN});

	//sm::timer tm{3'000, 300};
	//_monitor.subscribe(tm, sm::event{EPOLLIN});

	// poll
	while (_running == true) {
		_monitor.wait(*this);
	}
}


// -- private lifecycle -------------------------------------------------------

/* default constructor */
sm::taskmaster::taskmaster(const sm::options& opts)
:
  _config{},
  _running{true},
  _monitor{},
  _programs{},
  _executor{} {


	// set config from options
	if (opts.has_config() == true)
		_config.path(opts.config());

	// parse config
	_config.parse(_programs);

	// subscribe to signals
	_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});

	// become group leader
	::setpgid(0, 0);


	sm::logger::info("taskmaster: starting with pid: ", ::getpid());
}

/* destructor */
sm::taskmaster::~taskmaster(void) noexcept {

	// log
	sm::logger::warn("taskmaster: stopping");
}



// -- public static methods ---------------------------------------------------

/* run */
auto sm::taskmaster::run(const sm::options& opts) -> void {
	self::_shared(opts)._run();
}


// -- public accessors --------------------------------------------------------

/* monitor */
auto sm::taskmaster::monitor(void) noexcept -> sm::monitor& {
	return _monitor;
}

/* programs */
auto sm::taskmaster::programs(void) noexcept -> sm::program_manager& {
	return _programs;
}

/* executor */
auto sm::taskmaster::executor(void) noexcept -> sm::executor& {
	return _executor;
}

/* is running */
auto sm::taskmaster::is_running(void) const noexcept -> bool {
	return _running;
}


// -- public modifiers --------------------------------------------------------

/* stop */
auto sm::taskmaster::stop(void) noexcept -> void {
	_running = false;
}

/* reload */
auto sm::taskmaster::reload(void) -> void {

	sm::logger::warn("taskmaster: reloading not implemented yet");
}
