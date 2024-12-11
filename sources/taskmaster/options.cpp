#include "taskmaster/options.hpp"
#include "common/string/strlen.hpp"
#include "common/diagnostics/exception.hpp"
#include <limits.h>

#include <iostream>
#include <string.h>


// -- static file function ----------------------------------------------------

/* handle program */
static auto _handle_program(int ac, char** av) -> std::string {

	// check pointers
	if (ac < 1 || av == nullptr || *av == nullptr)
		throw sm::runtime_error{"command line arguments are broken"};

	// get program length
	auto len = sm::strlen(*av);

	// check emptyness
	if (len == 0U)
		throw sm::runtime_error{"argv[0] is empty"};

	sm::usize i = len;

	// loop over characters
	for (; i > 0U; --i) {

		if ((*av)[i - 1U] == '/')
			break;
	}

	// return basename
	return std::string{*av + i, len - i};
}


// -- O P T I O N S -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* argc / argv constructor */
sm::options::options(const int& ac, char** av)
: _program{_handle_program(ac, av)},
  _config{}, _port{4242}, _daemon{false} {

	// exit if no arguments
	if (ac == 1)
		return;

	// make iterator
	char**  it = av + 1;
	char** end = av + ac;

	// loop over argv
	for (; it != end; ++it) {

		// check pointer
		if (*it == nullptr)
			continue;

		// retrieve argument handler
		auto kf = self::_retrieve_handler(*it);

		if (kf.has_argument) {

			// check next argument
			if (++it == end || *it == nullptr)
				throw sm::runtime_error{"missing argument"};
		}

		// call handler
		(this->*kf.func)(*it);
	}

	std::cout << "port: " << _port << std::endl;
	std::cout << "daemon: " << _daemon << std::endl;
	std::cout << "config: " << _config << std::endl;
}

auto sm::options::_handle_port(const char* arg) -> void {

	// convert to integer
	_port = 0U;

	for (const char* it = arg; *it != '\0'; ++it) {

		if ((*it ^ '0') > 9)
			throw sm::runtime_error{"invalid port number"};

		// check multiplication overflow
		if (_port > (USHRT_MAX / 10U))
			throw sm::runtime_error{"port number overflow"};

		// shift port
		_port *= 10U;

		// get digit
		const auto digit = static_cast<::in_port_t>(*it - '0');

		// check addition overflow
		if (_port > (USHRT_MAX - digit))
			throw sm::runtime_error{"port number overflow"};

		// add digit
		_port += digit;
	}
}

auto sm::options::_handle_daemon(const char*) -> void {
	_daemon = true;
}

#include <unistd.h>

auto sm::options::_handle_config(const char* arg) -> void {

	// set config
	_config.assign(arg);

	//if (::access(_config.c_str(), R_OK) != 0)
	//	throw sm::runtime_error{"config file is not readable"};
}


// -- private methods ---------------------------------------------------------

/* retrieve handler */
auto sm::options::_retrieve_handler(const char* const arg) -> const key_function& {

	// loop over keys
	for (const auto& kf : _table) {

		// compare key
		if (::strcmp(kf.key, arg) == 0)
			return kf;
	}

	// not found
	throw sm::runtime_error{"invalid command line argument"};
}


// -- public accessors --------------------------------------------------------

/* port */
auto sm::options::port(void) const noexcept -> ::in_port_t {
	return _port;
}

/* daemon */
auto sm::options::daemon(void) const noexcept -> bool {
	return _daemon;
}

/* config */
auto sm::options::config(void) const noexcept -> const std::string& {
	return _config;
}
