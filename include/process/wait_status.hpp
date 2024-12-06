#ifndef wait_status_hpp
#define wait_status_hpp

#include <sys/wait.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {



	// -- W A I T  S T A T U S ------------------------------------------------

	class wait_status final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::wait_status;


			// -- private members ---------------------------------------------

			/* status */
			int _status;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			wait_status(void) noexcept;

			/* status constructor */
			wait_status(const int&) noexcept;

			/* copy constructor */
			wait_status(const self&) noexcept = default;

			/* move constructor */
			wait_status(self&&) noexcept = default;

			/* destructor */
			~wait_status(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* is terminated normally */
			auto is_terminated_normally(void) const noexcept -> bool;

			/* exit status */
			auto exit_status(void) const noexcept -> int;

			/* is signaled */
			auto is_signaled(void) const noexcept -> bool;

			/* signal */
			auto signal(void) const noexcept -> int;

			/* is core dumped */
			auto is_core_dumped(void) const noexcept -> bool;

			/* is stopped */
			auto is_stopped(void) const noexcept -> bool;

			/* stop signal */
			auto stop_signal(void) const noexcept -> int;

			/* is continued */
			auto is_continued(void) const noexcept -> bool;

	}; // class wait_status

} // namespace sm

#endif // wait_status_hpp
