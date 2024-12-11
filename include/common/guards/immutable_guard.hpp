#ifndef immutable_guard_hpp
#define immutable_guard_hpp

#include "common/diagnostics/exception.hpp"
#include <linux/fs.h>
#include <sys/ioctl.h>
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- I M M U T A B L E  G U A R D ----------------------------------------

	class immutable_guard final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::immutable_guard;


			// -- private members ---------------------------------------------

			/* file descriptor */
			int _fd;

			/* pid */
			int _pid;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			immutable_guard(void) = delete;

			/* descriptor constructor */
			immutable_guard(const int& fd)
			: _fd{fd}, _pid{::getpid()} {

				int attr = FS_IMMUTABLE_FL;

				// set immutable flag
				if (::ioctl(_fd, FS_IOC_SETFLAGS, &attr) == -1)
					throw sm::system_error("ioctl");
			}

			/* deleted copy constructor */
			immutable_guard(const self&) = delete;

			/* deleted move constructor */
			immutable_guard(self&&) = delete;

			/* destructor */
			~immutable_guard(void) noexcept {

				if (_pid != ::getpid())
					return;

				int attr = 0;

				// reset immutable flag
				static_cast<void>(::ioctl(_fd, FS_IOC_SETFLAGS, &attr) == -1);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;

	}; // class immutable_guard

} // namespace sm

#endif // immutable_guard_hpp
