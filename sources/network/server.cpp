#include "network/server.hpp"
#include "controller.hpp"


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

	if (events.is_in()) {

		// accept the client
		auto client = _socket.accept();
	}
}
