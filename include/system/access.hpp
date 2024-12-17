#ifndef access_hpp
#define access_hpp

#include "diagnostics/exception.hpp"
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* access */
	template <int mode>
	auto access(const char* path) -> void {

		// check access
		if (::access(path, mode) == -1)
			throw sm::system_error{"access"};
	}

} // namespace sm

#endif // access_hpp
