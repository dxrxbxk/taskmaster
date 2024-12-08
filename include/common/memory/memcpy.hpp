#ifndef taskmaster_memory_memcpy
#define taskmaster_memory_memcpy

#include "types.hpp"


// -- 4 2  N A M E S P A C E  -------------------------------------------------

namespace ft {


	/* memcpy */
	template <typename T>
	auto memcpy(T* dest, const T* src, const ft::umax& size) -> void {

		// copy memory
		static_cast<void>(__builtin_memcpy(dest, src, size * sizeof(T)));
	}

} // namespace ft

#endif // taskmaster_memory_memcpy
