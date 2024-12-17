#include "system/fork.hpp"
#include "diagnostics/exception.hpp"

#include <unistd.h>


/* fork */
auto sm::fork(void) -> ::pid_t {

	// fork process
	const ::pid_t pid = ::fork();

	if (pid == -1)
		throw sm::system_error("fork");

	return pid;
}
