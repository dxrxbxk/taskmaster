#ifndef syscall_hpp
#define syscall_hpp

#include "diagnostics/exception.hpp"

#include <unistd.h>


// -- S M  N A M E S P A C E  -------------------------------------------------

namespace sm {


	/* syscall */
	template <typename... Ts>
	auto syscall(const long& number, const Ts&... args) -> long {

		// call syscall
		const auto result = ::syscall(number, args...);

		// check for error
		if (result == -1)
			throw sm::system_error("syscall");

		return result;
	}

} // namespace sm

#endif // syscall_hpp
