#ifndef memcpy_hpp
#define memcpy_hpp

#include "types.hpp"
#include "config.hpp"

#if !sm_has_builtin(__builtin_memcpy)
#	include <string.h>
#endif


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* memcpy */
	template <typename T>
	auto memcpy(T* dst, const T* src, const sm::usize& size) -> T* {

		#if sm_has_builtin(__builtin_memcpy)
			return static_cast<T*>(__builtin_memcpy(dst, src, size * sizeof(T)));
		#else
			return static_cast<T*>(::memcpy(dst, src, size * sizeof(T)));
		#endif
	}

} // namespace sm

#endif // memcpy_hpp
