#ifndef memset_hpp
#define memset_hpp

#include "common/types.hpp"
#include "common/config/config.hpp"

#if !sm_has_builtin(__builtin_memset)
#	include <string.h>
#endif


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* memset */
	template <typename T>
	auto memset(T* dst, const T& value, const sm::usize& size) -> T* {

		#if sm_has_builtin(__builtin_memset)
			return static_cast<T*>(__builtin_memset(dst, value, size * sizeof(T)));
		#else
			return static_cast<T*>(::memset(dst, value, size * sizeof(T)));
		#endif
	}

} // namespace sm

#endif // memset_hpp
