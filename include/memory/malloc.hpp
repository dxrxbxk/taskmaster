#ifndef memory_malloc
#define memory_malloc

#include "types.hpp"
#include "diagnostics/exception.hpp"

#include <stdlib.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* malloc */
	template <typename T>
	auto malloc(const sm::usize& size = 1U) -> T* {

		T* ptr = static_cast<T*>(::malloc(size * sizeof(T)));

		if (ptr == nullptr)
			throw sm::system_error{"malloc"};

		return ptr;
	}

	/* realloc */
	template <typename T>
	auto realloc(T* ptr, const sm::usize& size) -> T* {

		T* new_ptr = static_cast<T*>(::realloc(ptr, size * sizeof(T)));

		if (new_ptr == nullptr)
			throw sm::system_error{"realloc"};

		return new_ptr;
	}

	/* calloc */
	template <typename T>
	auto calloc(const sm::usize& size = 1U) -> T* {

		T* ptr = static_cast<T*>(::calloc(size, sizeof(T)));

		if (ptr == nullptr)
			throw sm::system_error{"calloc"};

		return ptr;
	}

	/* free */
	template <typename T>
	auto free(T* ptr) -> void {
		::free(ptr);
	}

} // namespace sm

#endif // memory_malloc
