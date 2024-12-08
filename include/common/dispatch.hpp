#ifndef dispatch_hpp
#define dispatch_hpp

#include "common/types.hpp"
#include "common/resources/unique_fd.hpp"

#include <stdexcept>
#include <vector>

#include <sys/epoll.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E V E N T -----------------------------------------------------------

	class event final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::event;


			// -- private members ---------------------------------------------

			/* events */
			::uint32_t _events;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			event(void) noexcept = default;

			/* events constructor */
			explicit event(const ::uint32_t& events)
			: _events{events} {
			}

			/* copy constructor */
			event(const self&) noexcept = default;

			/* move constructor */
			event(self&&) noexcept = default;

			/* destructor */
			~event(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public conversion operators ---------------------------------

			/* uint32_t conversion operator */
			operator const ::uint32_t&(void) const noexcept {
				return _events;
			}


			// -- public accessors --------------------------------------------

			/* is in */
			auto is_in(void) const noexcept -> bool {
				return _events & EPOLLIN;
			}

			/* is out */
			auto is_out(void) const noexcept -> bool {
				return _events & EPOLLOUT;
			}

			/* is pri */
			auto is_pri(void) const noexcept -> bool {
				return _events & EPOLLPRI;
			}

			/* is rdhup */
			auto is_rdhup(void) const noexcept -> bool {
				return _events & EPOLLRDHUP;
			}

			/* is hup */
			auto is_hup(void) const noexcept -> bool {
				return _events & EPOLLHUP;
			}

			/* is err */
			auto is_err(void) const noexcept -> bool {
				return _events & EPOLLERR;
			}

			// EPOLLIN        // input data available
			// EPOLLPRI       // high-priority input data available
			// EPOLLOUT       // output data can be written
			// EPOLLRDNORM    // input data available (same as EPOLLIN, for compatibility)
			// EPOLLRDBAND    // input data available (same as EPOLLPRI, for compatibility)
			// EPOLLWRNORM    // output data can be written (same as EPOLLOUT)
			// EPOLLWRBAND    // output data can be written (same as EPOLLOUT, for compatibility)
			// EPOLLMSG       // input message available (since Linux 2.6.17)
			// EPOLLERR       // error condition happened
			// EPOLLHUP       // hang up happened, meaning the file descriptor is no longer connected
			// EPOLLRDHUP     // Stream socket peer closed connection, or shut down writing half of connection
			// EPOLLEXCLUSIVE //
			// EPOLLWAKEUP
			// EPOLLONESHOT
			// EPOLLET

	}; // class event


	// -- L I S T E N E R -----------------------------------------------------

	template <typename T>
	class listener {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			listener(void) noexcept = default;

			/* copy constructor */
			listener(const listener&) noexcept = default;

			/* move constructor */
			listener(listener&&) noexcept = default;

			/* destructor */
			virtual ~listener(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const listener&) noexcept -> listener& = default;

			/* move assignment operator */
			auto operator=(listener&&) noexcept -> listener& = default;


			// -- public interface --------------------------------------------

			/* fd */
			virtual auto fd(void) const noexcept -> int = 0;

			/* on event */
			virtual auto on_event(T&, const sm::event&) -> void = 0;

	}; // class event


	// -- M O N I T O R -------------------------------------------------------

	template <typename T>
	class monitor final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::monitor<T>;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* events */
			std::vector<struct ::epoll_event> _events;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			monitor(void)
			: _fd{::epoll_create1(0)} {

				if (_fd == -1)
					throw std::runtime_error("epoll_create1 failed");
			}

			/* deleted copy constructor */
			monitor(const self&) = delete;

			/* move constructor */
			monitor(self&&) noexcept = default;

			/* destructor */
			~monitor(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* subscribe */
			auto subscribe(sm::listener<T>& listener, const sm::event& events) -> void {

				// create event
				struct ::epoll_event ev {
					.events = events,
					.data = {
						.ptr = &listener
					}
				};

				// add event
				if (::epoll_ctl(_fd, EPOLL_CTL_ADD, listener.fd(), &ev) == -1)
					throw std::runtime_error("epoll_ctl failed");

				// add event to list
				_events.resize(_events.size() + 1U);
			}

			/* unsubscribe */
			auto unsubscribe(sm::listener<T>& listener) noexcept -> void {

				// delete event
				static_cast<void>(::epoll_ctl(_fd, EPOLL_CTL_DEL, listener.fd(), nullptr));
			}



			/* wait */
			auto wait(T& data) -> void {

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

					throw std::runtime_error("epoll_wait failed");
				}

				// loop over events
				for (sm::u32 i = 0U; i < static_cast<sm::u32>(state); ++i) {

					// get event
					const sm::event ev = sm::event{_events[i].events};

					// get user data
					auto& listener = *(reinterpret_cast<sm::listener<T>*>(_events[i].data.ptr));

					// trigger event
					listener.on_event(data, ev);
				}

			}

	}; // class monitor

} // namespace sm

#endif // dispatch_hpp

