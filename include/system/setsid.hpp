#ifndef system_setsid
#define system_setsid

#include <unistd.h>
#include <stdexcept>


// -- S E R V I C E  M A N A G E R --------------------------------------------

namespace sm {


	/* setsid */
	inline auto setsid(void) -> ::pid_t {

		const auto sid = ::setsid();

		if (sid == -1)
			throw std::runtime_error("setsid failed");

		return sid;
	}

} // namespace sm

#endif // system_setsid
