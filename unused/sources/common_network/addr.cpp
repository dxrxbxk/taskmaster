#include "network/addr.hpp"


// -- A D D R -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::addr::addr(void) noexcept
: _storage{}, _size{sizeof(::sockaddr_storage)} {
}

/* ipv4 constructor */
sm::addr::addr(const ::in_addr& ip, const ::in_port_t& port) noexcept
: _storage{}, _size{sizeof(::sockaddr_in)} {

	auto& storage = self::as_ipv4();

	storage.sin_family = AF_INET;
	storage.sin_port = ::htons(port);
	storage.sin_addr.s_addr = ip.s_addr;
}

/* ipv6 constructor */
sm::addr::addr(const ::in6_addr& ip, const ::in_port_t& port) noexcept
: _storage{}, _size{sizeof(::sockaddr_in6)} {

	auto& storage = self::as_ipv6();

	storage.sin6_family = AF_INET6;
	storage.sin6_port = ::htons(port);
	storage.sin6_addr = ip;
}

/* local constructor */
sm::addr::addr(const char* path) noexcept
: _storage{}, _size{sizeof(::sockaddr_un)} {

	auto& storage = self::as_local();

	storage.sun_family = AF_UNIX;

	if (path == nullptr)
		return;

	::size_t i = 0U;

	// loop over the path
	while (path[i] != '\0') {

		// check if we reached the end of the storage array
		if (i == sizeof(storage.sun_path) - 1U)
			break;

		// copy character
		storage.sun_path[i] = path[i];
	}

	// null-terminate the path
	storage.sun_path[i] = '\0';
}


// -- public accessors --------------------------------------------------------

/* size */
auto sm::addr::size(void) noexcept -> ::socklen_t& {
	return _size;
}

/* const size */
auto sm::addr::size(void) const noexcept -> const ::socklen_t& {
	return _size;
}

/* as ipv4 */
auto sm::addr::as_ipv4(void) noexcept -> ::sockaddr_in& {
	return *reinterpret_cast<::sockaddr_in*>(&_storage);
}

/* const as ipv4 */
auto sm::addr::as_ipv4(void) const noexcept -> const ::sockaddr_in& {
	return *reinterpret_cast<const ::sockaddr_in*>(&_storage);
}

/* as ipv6 */
auto sm::addr::as_ipv6(void) noexcept -> ::sockaddr_in6& {
	return *reinterpret_cast<::sockaddr_in6*>(&_storage);
}

/* const as ipv6 */
auto sm::addr::as_ipv6(void) const noexcept -> const ::sockaddr_in6& {
	return *reinterpret_cast<const ::sockaddr_in6*>(&_storage);
}

/* as local */
auto sm::addr::as_local(void) noexcept -> ::sockaddr_un& {
	return *reinterpret_cast<::sockaddr_un*>(&_storage);
}

/* const as local */
auto sm::addr::as_local(void) const noexcept -> const ::sockaddr_un& {
	return *reinterpret_cast<const ::sockaddr_un*>(&_storage);
}

/* as sockaddr */
auto sm::addr::as_sockaddr(void) noexcept -> ::sockaddr& {
	return *reinterpret_cast<::sockaddr*>(&_storage);
}

/* const as sockaddr */
auto sm::addr::as_sockaddr(void) const noexcept -> const ::sockaddr& {
	return *reinterpret_cast<const ::sockaddr*>(&_storage);
}
