#include "taskmaster/taskmaster.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/signal.hpp"
#include "common/running.hpp"
#include "common/system/fork.hpp"
#include "common/system/setsid.hpp"
#include <sys/stat.h>


// -- T A S K M A S T E R -----------------------------------------------------

// -- private static methods --------------------------------------------------

/* shared */
auto sm::taskmaster::_shared(void) -> self& {
	static self instance;
	return instance;
}


// -- private lifecycle -------------------------------------------------------

auto sm::taskmaster::_run(void) -> void {

	// poll
	while (sm::running::state() == true) {

		// wait for events
		_monitor.wait();
	}
}

/* default constructor */
sm::taskmaster::taskmaster(void)
: _server{::in_port_t{4242}},
  _monitor{},
  _clients{} {

	// subscribe to server
	_monitor.subscribe(_server, sm::event{EPOLLIN});

	// subscribe to signals
	_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});

}


// -- public static methods ---------------------------------------------------

/* run */
auto sm::taskmaster::run(void) -> void {
	//static_cast<void>(self::_shared());
	self::_shared()._run();
}

/* server */
auto sm::taskmaster::server(void) noexcept -> sm::server& {
	return self::_shared()._server;
}

/* monitor */
auto sm::taskmaster::monitor(void) noexcept -> sm::monitor& {
	return self::_shared()._monitor;
}

/* stop */
auto sm::taskmaster::stop(void) noexcept -> void {
	sm::running::stop();
}

/* clients */
auto sm::taskmaster::clients(void) noexcept -> sm::client_manager& {
	return self::_shared()._clients;
}
