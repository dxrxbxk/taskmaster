#ifndef fork_hpp
#define fork_hpp

#include <sys/types.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	/* fork */
	auto fork(void) -> ::pid_t;
}

#endif // TASKMASTER_SYSTEM_FORK_HPP
