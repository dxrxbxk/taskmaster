#ifndef dispatch_hpp
#define dispatch_hpp

#include "os.hpp"
#include "types.hpp"
#include "system/unique_fd.hpp"
#include <stdexcept>
#include <vector>

#include "io_event.hpp"

#include <sys/epoll.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- D I S P A T C H -----------------------------------------------------

	class dispatch final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::dispatch;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

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
			auto add(sm::io_event& io, const ::uint32_t& events) -> void {

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
			auto del(sm::io_event& io) -> void {

				// delete event
				if (::epoll_ctl(_fd, EPOLL_CTL_DEL, io.fd(), nullptr) == -1)
					throw std::runtime_error("kevent failed");

				// remove event from list
				_events.pop_back();
			}

			/* del (noexcept) */
			auto del_noexcept(sm::io_event& io) noexcept -> void {

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
				for (sm::u32 i = 0U; i < static_cast<sm::u32>(state); ++i) {

					// get event
					const auto& event = _events[i];

					// get user data
					auto& data = *(reinterpret_cast<sm::io_event*>(_events[i].data.ptr));

					// trigger event
					data.on_event(event.events);
				}

			}

	}; // class dispatch

} // namespace sm

#endif // dispatch_hpp

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
