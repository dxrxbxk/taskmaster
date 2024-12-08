#include "common/network/socket.hpp"
#include "common/network/addr.hpp"

#include <fcntl.h>
#include <stdexcept>


// -- S O C K E T -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* socket constructor */
sm::socket::socket(const int& domain, const int& type, const int& protocol) 
: sm::unique_fd(::socket(domain, type, protocol)) {

	if (_fd == -1)
		throw std::runtime_error("failed to create socket");
}


// -- public conversion operators ---------------------------------------------

/* int conversion operator */
sm::socket::operator const int&(void) const noexcept {
	return unique_fd::operator const int&();
}


// -- public methods ----------------------------------------------------------

/* bind */
auto sm::socket::bind(const sm::addr& addr) const -> void {

	// bind the socket to the address
	if (::bind(_fd, &addr.as_sockaddr(), addr.size()) == -1)
		throw std::runtime_error("failed to bind socket");
}

/* listen */
auto sm::socket::listen(int backlog) const -> void {

	// listen on the socket
	if (::listen(_fd, backlog) == -1)
		throw std::runtime_error("failed to listen on socket");
}

/* accept */
auto sm::socket::accept(sm::addr &addr) const -> self {

	self client;

	client._fd = ::accept(_fd, &addr.as_sockaddr(), &addr.size());

	if (client._fd == -1)
		throw std::runtime_error("failed to accept");

	return client;
}

/* accept */
auto sm::socket::accept(void) const -> self {

	self client;

	client._fd = ::accept(_fd, nullptr, nullptr);

	if (client._fd == -1)
		throw std::runtime_error("failed to accept");

	return client;
}

/* connect */
auto sm::socket::connect(const sm::addr& addr) const -> void {

	// connect to the address
	if (::connect(_fd, &addr.as_sockaddr(), addr.size()) == -1)
		throw std::runtime_error("failed to connect to address");
}

/* non-blocking */
auto sm::socket::non_blocking(void) const -> void {

	// set the socket to non-blocking mode
	if (::fcntl(_fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("failed to set socket to non-blocking mode");

}

/* shutdown */
auto sm::socket::shutdown(const int& how) const -> void {

	// shutdown the socket
	if (::shutdown(_fd, how) == -1)
		throw std::runtime_error("failed to shutdown socket");
}

/* reuse address */
auto sm::socket::reuse_address(void) const -> void {

	int opt = 1;

	// set the socket option
	if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		throw std::runtime_error{"failed to set socket option"};
}

/* receive */
auto sm::socket::receive(char* buffer, const size_t& size) const -> ssize_t {

	// receive data
	const auto bytes = ::recv(_fd, buffer, size, 0);

	if (bytes == -1)
		throw std::runtime_error("failed to receive data");

	return bytes;
}

/* timeout */
auto sm::socket::timeout(const long& ms, const int& opt) const -> void {

	const struct ::timeval tv {
		.tv_sec = ms / 1000,
		.tv_usec = (ms % 1000) * 1000
	};

	// set the socket option
	if (::setsockopt(_fd, SOL_SOCKET, opt, &tv, sizeof(tv)) == -1)
		throw std::runtime_error{"failed to set socket option"};
}
