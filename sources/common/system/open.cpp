#include "common/system/open.hpp"
#include "common/diagnostics/exception.hpp"

/* open */
auto sm::open(const char* path, const int& flags) -> int {

	const auto fd = ::open(path, flags);

	if (fd == -1)
		throw sm::system_error("open");

	return fd;
}

/* open */
auto sm::open(const char* path, const int& flags, const ::mode_t mode) -> int {

	const auto fd = ::open(path, flags, mode);

	if (fd == -1)
		throw sm::system_error("open");

	return fd;
}
