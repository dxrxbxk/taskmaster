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
			virtual auto fd(void) const noexcept -> int = 0;

			/* on event */
			virtual auto on_event(const ::uint32_t&) -> void = 0;



// EPOLLIN
// EPOLLPRI
// EPOLLOUT
// EPOLLRDNORM
// EPOLLRDBAND
// EPOLLWRNORM
// EPOLLWRBAND
// EPOLLMSG
// EPOLLERR
// EPOLLHUP
// EPOLLRDHUP
// EPOLLEXCLUSIVE
// EPOLLWAKEUP
// EPOLLONESHOT
// EPOLLET


	}; // class io_event



	// -- D I S P A T C H -----------------------------------------------------

	class dispatch final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sys::dispatch;


			// -- private members ---------------------------------------------

			/* file descriptor */
			ft::unique_fd _fd;

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

			/* deleted copy constructor */
			dispatch(const self&) = delete;

			/* move constructor */
			dispatch(self&&) noexcept = default;

			/* destructor */
			~dispatch(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* add */
			auto add(sys::io_event& io, const ::uint32_t& events) -> void {

				// create event
				struct ::epoll_event ev {
					.events = events,
					.data = {
						.ptr = &io
					}
				};

				// add event
				if (::epoll_ctl(_fd, EPOLL_CTL_ADD, io.fd(), &ev) == -1)
					throw std::runtime_error("kevent failed");

				// add event to list
				_events.resize(_events.size() + 1U);
			}

			/* del */
			auto del(sys::io_event& io) -> void {

				// delete event
				if (::epoll_ctl(_fd, EPOLL_CTL_DEL, io.fd(), nullptr) == -1)
					throw std::runtime_error("kevent failed");

				// remove event from list
				_events.pop_back();
			}

			/* del (noexcept) */
			auto del_noexcept(sys::io_event& io) noexcept -> void {

				// delete event
				static_cast<void>(::epoll_ctl(_fd, EPOLL_CTL_DEL, io.fd(), nullptr));
			}


			/* wait */
			auto wait(void) -> void {

				// wait for events
				const auto state = ::epoll_wait(_fd,
												_events.data(),
												static_cast<int>(_events.size()),
												-1);

				// check for error
				if (state == -1) {

					// check for interruption
					if (errno == EINTR || errno == EAGAIN)
						return;

					throw std::runtime_error("kevent failed");
				}

				// loop over events
				for (ft::umax i = 0U; i < static_cast<ft::umax>(state); ++i) {

					// get event
					const auto& event = _events[i];

					// get user data
					auto& data = *(reinterpret_cast<sys::io_event*>(_events[i].data.ptr));

					// trigger event
					data.on_event(event.events);
				}

			}

	};

} // namespace tm

#endif // TASKMASTER_DISPATCH_HPP
