#ifndef dispatch_hpp
#define dispatch_hpp

#include "types.hpp"
#include "diagnostics/exception.hpp"
#include "resources/unique_fd.hpp"

#include <vector>

#include <sys/epoll.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* taskmaster */
	class taskmaster;


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
			virtual auto on_event(const sm::event&, sm::taskmaster&) -> void = 0;

	}; // class event



	// -- M O N I T O R -------------------------------------------------------

	class monitor final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::monitor;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* events */
			std::vector<struct ::epoll_event> _events;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			monitor(void);

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
			auto subscribe(sm::listener&, const sm::event&) -> void;

			/* unsubscribe */
			auto unsubscribe(sm::listener&) noexcept -> void;

			/* wait */
			auto wait(sm::taskmaster&) -> void;

	}; // class monitor

} // namespace sm

#endif // dispatch_hpp
