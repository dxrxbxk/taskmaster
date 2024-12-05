/* ************************************************************************* */
/*                                                                           */
/*            ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖              */
/*              █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌             */
/*              █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖             */
/*              █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌             */
/*                                                                           */
/* ************************************************************************* */

#ifndef TASKMASTER_DISPATCH_HPP
#define TASKMASTER_DISPATCH_HPP

#include "os.hpp"
#include "types.hpp"
#include "system/unique_fd.hpp"
#include <stdexcept>
#include <vector>

#if defined(os_macos)
#	include <sys/event.h>
#elif defined(os_linux)
#	include <sys/epoll.h>
#endif


// -- S Y S T E M  N A M E S P A C E ------------------------------------------

namespace sys {


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
			virtual auto fd(void) const noexcept -> const int& = 0;

			/* in */
			virtual auto in(void) -> void = 0;

			/* out */
			virtual auto out(void) -> void = 0;

			/* error */
			virtual auto error(void) -> void = 0;

			/* hangup */
			virtual auto hangup(void) -> void = 0;


	}; // class io_event



	// -- D I S P A T C H -----------------------------------------------------

	class dispatch final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sys::dispatch;


			// -- private members ---------------------------------------------

			/* file descriptor */
			tm::unique_fd _fd;

			/* events */
			std::vector<struct ::epoll_event> _events;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			dispatch(void)
			: _fd{::epoll_create1(0)} {

				if (_fd == -1)
					throw std::runtime_error("kqueue failed");
			}


			/* wait */
			auto wait(void) -> void {

				// wait for events
				const auto state = ::epoll_wait(_fd, _events.data(), _events.size(), -1);


				// check for error
				if (state == -1) {

					// check for interruption
					if (errno == EINTR || errno == EAGAIN)
						return;

					throw std::runtime_error("kevent failed");
				}

				// iterate over events
				for (sm::umax i = 0U; i < static_cast<sm::umax>(state); ++i) {

					// get event
					const auto& event = _events[i];

					// get user data
					auto& data = *(reinterpret_cast<sys::io_event*>(event.data.ptr));
				}

			}





	};

} // namespace tm

#endif // TASKMASTER_DISPATCH_HPP
