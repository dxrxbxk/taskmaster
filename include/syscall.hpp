#ifndef TASKMASTER_SYSTEM_SYSCALL_HPP
#define TASKMASTER_SYSTEM_SYSCALL_HPP

#include <unistd.h>
#include <stdexcept>


// -- 4 2  N A M E S P A C E  -------------------------------------------------

namespace ft {


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

} // namespace ft

#endif // TASKMASTER_SYSTEM_SYSCALL_HPP
