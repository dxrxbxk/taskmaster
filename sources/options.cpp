#include "options.hpp"
#include "string/strlen.hpp"
#include "diagnostics/exception.hpp"
#include <limits.h>

#include <iostream>
#include <string.h>


// -- O P T I O N S -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* argc / argv constructor */
sm::options::options(const int& ac, char** av)
: _config{} {

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

}

/* handle config */
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

/* config */
auto sm::options::config(void) const noexcept -> const std::string& {
	return _config;
}

/* has config */
auto sm::options::has_config(void) const noexcept -> bool {
	return _config.empty() == false;
}
