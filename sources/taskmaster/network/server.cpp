#include "taskmaster/network/server.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/taskmaster.hpp"

#include <string>

// -- S E R V E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* port constructor */
sm::server::server(const ::in_port_t& port)
: _socket{AF_INET, SOCK_STREAM} {

	// bind the socket
	_socket.bind(sm::addr{::in_addr{INADDR_ANY}, port});

	// listen on the socket
	_socket.listen(SOMAXCONN);

	// set non-blocking
	_socket.non_blocking();

	// set reuse address
	_socket.reuse_address();

	std::string msh = "server: listening on port " + std::to_string(port);

	sm::logger::info(msh.c_str());
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::server::fd(void) const noexcept -> int {
	return _socket;
}

/* on event */
auto sm::server::on_event(const sm::event& events) -> void {

	auto& monitor = sm::taskmaster::monitor();
	auto& clients = sm::taskmaster::clients();

	if (events.is_in()) {

		sm::logger::info("server: incoming connection");

		// accept the client
		auto socket = _socket.accept();

		// create new client
		auto& client = clients.add(static_cast<sm::socket&&>(socket));

		// subscribe to events
		monitor.subscribe(client, sm::event{EPOLLIN | EPOLLRDHUP | EPOLLHUP});
	}
}
