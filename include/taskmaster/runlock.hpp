#ifndef runlock_hpp
#define runlock_hpp

#include "common/resources/unique_fd.hpp"
#include "common/guards/lock_guard.hpp"
#include "common/guards/immutable_guard.hpp"
#include "common/system/open.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R U N L O C K -------------------------------------------------------

	class runlock final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::runlock;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* lock guard */
			sm::lock_guard _guard;

			/* immutable guard */
			sm::immutable_guard _immutable;


			// -- private statis methods --------------------------------------

			/* open */
			static auto _open(void) -> int {

				// create path if not exists
				// '/var/lock/taskmaster/'


				// create file
				// '/var/lock/taskmaster/taskmaster.lock'

				// to be implemented...

				return sm::open("./taskmaster.lock", O_CREAT, 0000);
			}

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runlock(void)
			: _fd{self::_open()},
			  _guard{_fd},
			  _immutable{_fd} {
			}

			/* deleted copy constructor */
			runlock(const self&) = delete;

			/* deleted move constructor */
			runlock(self&&) = delete;

			/* destructor */
			~runlock(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;

	}; // class runlock

} // namespace sm

#endif // runlock_hpp
