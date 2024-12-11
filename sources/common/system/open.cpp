#include "common/system/open.hpp"
#include "common/diagnostics/exception.hpp"


// octal specifier | user | group | others
// 0: no permission
// 1: execute only
// 2: write only
// 3: write and execute
// 4: read only
// 5: read and execute
// 6: read and write
// 7: read, write, execute


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
