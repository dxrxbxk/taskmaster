#ifndef syscall_hpp
#define syscall_hpp

#include <unistd.h>
#include <stdexcept>


// -- S M  N A M E S P A C E  -------------------------------------------------

namespace sm {


	/* syscall */
	template <typename... Ts>
	auto syscall(const long& number, const Ts&... args) -> long {

		// call syscall
		const auto result = ::syscall(number, args...);

		// check for error
		if (result == -1)
			throw std::runtime_error("syscall failed");

		return result;
	}

} // namespace sm

#endif // syscall_hpp
