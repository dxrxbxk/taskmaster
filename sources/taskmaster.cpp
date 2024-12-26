#include "options.hpp"
#include "taskmaster.hpp"
#include "log/logger.hpp"
#include "signal.hpp"

#include "time/timer.hpp"
#include "pid.hpp"



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

	_readline.prompt();

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
  _inotify{},
  _programs{},
  _executor{},
  _readline{} {

	// initialize pid
	static_cast<void>(sm::pid::get());

	// set config from options
	if (opts.has_config() == true)
		_config.path(opts.config());

	// parse config
	_config.parse(_programs);

	// subscribe to signals
	_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});

	// subscribe to readline
	_monitor.subscribe(_readline, sm::event{EPOLLIN});

	// subscribe to inotify
	_monitor.subscribe(_inotify, sm::event{EPOLLIN});


	// watch config file
	_inotify.watch(_config, IN_MODIFY
						  | IN_DELETE_SELF
						  | IN_MOVE_SELF
						  | IN_ONESHOT);


	sm::logger::info("taskmaster: starting with pid: ", ::getpid());

	// run auto-start programs
	_programs.autostart(_monitor);
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

/* readline */
auto sm::taskmaster::readline(void) const noexcept -> const sm::readline& {
	return _readline;
}

/* inotify */
auto sm::taskmaster::inotify(void) noexcept -> sm::inotify& {
	return _inotify;
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

	_config.reload(*this);

	_programs.autostart(_monitor);
}
