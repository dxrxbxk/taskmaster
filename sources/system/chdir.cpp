#include "system/chdir.hpp"
#include "diagnostics/exception.hpp"
#include <unistd.h>


/* chdir */
auto sm::chdir(const char* path) -> void {

	// change directory
	if (::chdir(path) == -1)
		throw sm::system_error("chdir");
}
