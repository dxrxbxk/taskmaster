#ifndef system_setsid
#define system_setsid

#include "diagnostics/exception.hpp"

#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* setsid */
	inline auto setsid(void) -> ::pid_t {

		const auto sid = ::setsid();

		if (sid == -1)
			throw sm::system_error("setsid");

		return sid;
	}

} // namespace sm

#endif // system_setsid
