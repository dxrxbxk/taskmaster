#ifndef limits_hpp
#define limits_hpp

#include "type_traits/type_traits.hpp"
#include "types.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- L I M I T S ---------------------------------------------------------

	template <typename T>
	class limits final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::limits<T>;



		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			limits(void) = delete;

			/* deleted copy constructor */
			limits(const self&) = delete;

			/* deleted move constructor */
			limits(self&&) = delete;

			/* destructor */
			~limits(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public static methods ---------------------------------------

			/* max */
			static consteval auto max(void) noexcept -> T requires (sm::is_integral<T>
																&& !sm::is_bool<T>) {

				// number of bits in type
				constexpr T bits = static_cast<T>((sizeof(T) * 8U) - sm::is_signed<T>);

				T count = 0;

				// loop through bits
				for (T i = 0; i < bits; ++i)
					// add shifted bit to max
					count |= static_cast<T>(static_cast<T>(1) << i);

				return count;
			}

			/* min */
			static consteval auto min(void) noexcept -> T requires (sm::is_integral<T>
																&& !sm::is_bool<T>) {

				//constexpr T max = self::max();
				//return ~max;
				return static_cast<T>(~(self::max()));
			}

			/* max (bool) */
			static consteval auto max(void) noexcept -> T requires sm::is_bool<T> {
				return true;
			}

			/* min (bool) */
			static consteval auto min(void) noexcept -> T requires sm::is_bool<T> {
				return false;
			}

			/* digits */
			template <unsigned base = 10U>
			static consteval auto digits(void) -> sm::usize {

				// max value of type
				T value = self::max();

				// number of digits
				sm::usize digits = 0U;

				do {

					// increment digits
					++digits;

				// divide max by base
				} while (value /= base);

				return digits;
			}

	}; // class limits

} // namespace sm

#endif // limits_hpp
