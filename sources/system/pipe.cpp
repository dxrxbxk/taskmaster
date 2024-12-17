#include "system/pipe.hpp"
#include "diagnostics/exception.hpp"

#include <unistd.h>


/* pipe */
auto sm::pipe(int (&fds)[2U]) -> void {

	// create pipe
	if (::pipe(fds) == -1)
		throw sm::system_error("pipe");
}
