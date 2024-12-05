#ifndef TASKMASTER_SYSTEM_FORK_HPP
#define TASKMASTER_SYSTEM_FORK_HPP

#include <sys/types.h>


// -- 4 2  N A M E S P A C E  -------------------------------------------------

namespace ft {

	/* fork */
	auto fork(void) -> ::pid_t;
}

#endif // TASKMASTER_SYSTEM_FORK_HPP
