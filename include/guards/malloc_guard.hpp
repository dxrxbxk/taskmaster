#ifndef malloc_guard_hpp
#define malloc_guard_hpp

#include "memory/malloc.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- M A L L O C  G U A R D ----------------------------------------------

	class malloc_guard final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::malloc_guard;


			// -- private members ---------------------------------------------

			/* pointer to data */
			void* _data;

			/* complete flag */
			bool _complete;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			malloc_guard(void) = delete;

			/* data constructor */
			malloc_guard(void* data) noexcept
			: _data{data}, _complete{false} {
			}

			/* deleted copy constructor */
			malloc_guard(const self&) = delete;

			/* move constructor */
			malloc_guard(self&& other) noexcept
			: _data{other._data}, _complete{other._complete} {

				// invalidate other
				other._complete = true;
			}

			/* destructor */
			~malloc_guard(void) noexcept {

				// check for complete
				if (_complete)
					return;

				// deallocate memory
				sm::free(_data);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				if (not _complete)
					sm::free(_data);

				// move data
				_data = other._data;
				_complete = other._complete;

				// invalidate other
				other._complete = true;

				// done
				return *this;
			}


			// -- public modifiers --------------------------------------------

			/* complete */
			auto complete(void) noexcept -> void {
				_complete = true;
			}

	}; // class malloc_guard

} // namespace sm

#endif // malloc_guard_hpp
