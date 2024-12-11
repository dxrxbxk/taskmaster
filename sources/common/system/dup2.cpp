#include "common/system/dup2.hpp"
#include "common/diagnostics/exception.hpp"
#include <unistd.h>


/* dup2 */
auto sm::dup2(const int& oldfd, const int& newfd) -> void {

	// try to duplicate
	if (::dup2(oldfd, newfd) == -1)
		throw sm::system_error("dup2");
}
