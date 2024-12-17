#ifndef sysconf_hpp
#define sysconf_hpp

#include "diagnostics/exception.hpp"
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* sysconf */
	template <int name>
	auto sysconf(void) -> long {

		// get sysconf
		const auto ret = ::sysconf(name);

		// check error
		if (ret == -1)
			throw sm::system_error{"sysconf"};

		return ret;
	}

} // namespace sm

#endif // sysconf_hpp
