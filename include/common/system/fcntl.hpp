#ifndef fcntl_hpp
#define fcntl_hpp

#include "common/diagnostics/exception.hpp"
#include <fcntl.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* fcntl */
	template <typename... Ts>
	auto fcntl(const int& fd, const int& cmd, const Ts&... args) -> int {

		// call fcntl
		const auto ret = ::fcntl(fd, cmd, args...);

		// check for error
		if (ret == -1)
			throw sm::system_error("fcntl");

		// return result
		return ret;
	}

} // namespace sm

#endif // fcntl_hpp
