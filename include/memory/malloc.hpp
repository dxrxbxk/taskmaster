#ifndef memory_malloc
#define memory_malloc

#include <stdlib.h>
#include <stdexcept>


// -- S E R V I C E  M A N A G E R --------------------------------------------

namespace sm {


	/* malloc */
	template <typename T>
	auto malloc(const ::size_t& size) -> T* {

		T* ptr = static_cast<T*>(::malloc(size * sizeof(T)));

		if (ptr == nullptr)
			throw std::bad_alloc();

		return ptr;
	}

	/* realloc */
	template <typename T>
	auto realloc(T* ptr, const ::size_t& size) -> T* {

		T* new_ptr = static_cast<T*>(::realloc(ptr, size * sizeof(T)));

		if (new_ptr == nullptr)
			throw std::bad_alloc();

		return new_ptr;
	}

	/* free */
	template <typename T>
	auto free(T* ptr) -> void {
		::free(ptr);
	}

} // namespace sm

#endif // memory_malloc
