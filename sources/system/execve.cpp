#include "system/execve.hpp"
#include "diagnostics/exception.hpp"

#include <unistd.h>


/* execve */
auto sm::execve(const char* path, char* const* argv, char* const* envp) -> void {

	if (::execve(path, argv, envp) == -1)
		throw sm::system_error{"execve"};
}
