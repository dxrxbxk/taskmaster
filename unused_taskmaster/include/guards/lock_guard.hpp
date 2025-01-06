#ifndef lock_guard_hpp
#define lock_guard_hpp

#include "diagnostics/exception.hpp"
#include <sys/file.h>
#include <unistd.h>


#include <iostream>

// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- L O C K  G U A R D --------------------------------------------------

	class lock_guard final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::lock_guard;


			// -- private members ---------------------------------------------

			/* file descriptor */
			int _fd;

			/* pid */
			int _pid;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			lock_guard(void) = delete;

			/* descriptor constructor */
			lock_guard(const int& fd)
			: _fd{fd}, _pid{::getpid()} {

				// try to lock file
				if (::flock(_fd, LOCK_EX | LOCK_NB) == -1)
					throw sm::system_error("flock");
			}

			/* deleted copy constructor */
			lock_guard(const self&) = delete;

			/* deleted move constructor */
			lock_guard(self&&) = delete;

			/* destructor */
			~lock_guard(void) noexcept {

				// check for pid (fork)
				if (_pid != ::getpid())
					return;

				// unlock file
				static_cast<void>(::flock(_fd, LOCK_UN));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;

	}; // class lock_guard

} // namespace sm

#endif // lock_guard_hpp
