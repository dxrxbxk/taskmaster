#include "taskmaster/server.hpp"
#include "taskmaster/controller.hpp"

#include <iostream>


// -- S E R V E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* port constructor */
sm::server::server(const ::in_port_t& port)
: _socket{AF_INET, SOCK_STREAM} {

	// bind the socket
	_socket.bind(sm::addr{::in_addr{INADDR_ANY}, port});

	// listen on the socket
	_socket.listen(SOMAXCONN);
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::server::fd(void) const noexcept -> int {
	return _socket;
}

/* on event */
auto sm::server::on_event(sm::controller& controller,
						  const sm::event& events) -> void {

	auto& monitor = controller.monitor();
	auto& clients = controller.clients();

	if (events.is_in()) {

		std::cout << "server: incoming connection" << std::endl;

		// accept the client
		auto socket = _socket.accept();

		// create new client
		auto& client = clients.add(static_cast<sm::socket&&>(socket));

		// subscribe to events
		monitor.subscribe(client, sm::event{EPOLLIN | EPOLLRDHUP | EPOLLHUP});
	}
}
