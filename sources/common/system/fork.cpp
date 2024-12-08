#include "common/system/fork.hpp"
#include <unistd.h>
#include <stdexcept>


auto sm::fork(void) -> ::pid_t {

	// fork process
	const ::pid_t pid = ::fork();

	if (pid == -1)
		throw std::runtime_error("fork failed");

	return pid;
}
