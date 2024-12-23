#ifndef optional_hpp
#define optional_hpp

#include <type_traits>
#include <utility>
#include "memory/memcpy.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- O P T I O N A L -----------------------------------------------------

	template <typename T>
	class optional final {


		private:

			// -- private types ------------------------------------------------

			/* self type */
			using self = sm::optional<T>;


			// -- private members ---------------------------------------------

			/* storage */
			alignas(T) unsigned char _storage[sizeof(T)];

			/* engaged */
			bool _engaged;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			constexpr optional(void) noexcept
			: /* uninitialized storage */ _engaged{false} {
			}

			/* copy constructor */
			constexpr optional(const self& other)
			: _engaged{other._engaged} {

				// copy construct
				if (_engaged != true)
					return;

				if constexpr (std::is_trivially_copyable_v<T>)
					sm::memcpy(_storage, other._storage, sizeof(T));
				else
					::new(_storage) T{*reinterpret_cast<const T*>(other._storage)};
			}


			/* move constructor */
			constexpr optional(self&& other) noexcept
			: _engaged{other._engaged} {

				// move construct
				if (_engaged != true)
					return;

				if constexpr (std::is_trivially_copyable_v<T>)
					sm::memcpy(_storage, other._storage, sizeof(T));
				else
					::new(_storage) T{std::move(*reinterpret_cast<T*>(other._storage))};
			}

			/* destructor */
			~optional(void) noexcept requires (!std::is_trivially_destructible_v<T>) {

				if (_engaged == true)
					reinterpret_cast<T*>(_storage)->~T();
			}

			/* destructor */
			~optional(void) noexcept requires (std::is_trivially_destructible_v<T>) = default;


	}; // class optional

} // namespace sm



#endif // optional_hpp
