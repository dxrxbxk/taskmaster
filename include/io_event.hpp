#ifndef io_event_hpp
#define io_event_hpp

#include <stdint.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- I O  E V E N T ------------------------------------------------------

	class io_event {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			io_event(void) noexcept = default;

			/* copy constructor */
			io_event(const io_event&) noexcept = default;

			/* move constructor */
			io_event(io_event&&) noexcept = default;

			/* destructor */
			virtual ~io_event(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const io_event&) noexcept -> io_event& = default;

			/* move assignment operator */
			auto operator=(io_event&&) noexcept -> io_event& = default;


			// -- public interface --------------------------------------------

			/* fd */
			virtual auto fd(void) const noexcept -> int = 0;

			/* on event */
			virtual auto on_event(const ::uint32_t&) -> void = 0;

	}; // class io_event

} // namespace sm

#endif // io_event_hpp
