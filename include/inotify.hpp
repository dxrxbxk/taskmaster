#ifndef inotify_hpp
#define inotify_hpp

#include "events/monitor.hpp"
#include "resources/unique_fd.hpp"
#include "diagnostics/exception.hpp"
#include <sys/inotify.h>
#include <unistd.h>
#include <unordered_map>

#include "time/timer.hpp"

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- N O T I F I A B L E -------------------------------------------------

	class notifiable {

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			notifiable(void) noexcept = default;

			/* copy constructor */
			notifiable(const notifiable&) noexcept = default;

			/* move constructor */
			notifiable(notifiable&&) noexcept = default;

			/* destructor */
			virtual ~notifiable(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const notifiable&) noexcept -> notifiable& = default;

			/* move assignment operator */
			auto operator=(notifiable&&) noexcept -> notifiable& = default;


			// -- public interface --------------------------------------------

			/* path */
			virtual auto path(void) const noexcept -> const char* = 0;

			/* on event */
			virtual auto on_event(const ::uint32_t&, sm::taskmaster&) -> void = 0;

	}; // class notifiable



	// -- I N O T I F Y -------------------------------------------------------

	class inotify final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::inotify;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* map */
			std::unordered_map<int, sm::notifiable*> _map;

			/* events map */
			std::unordered_map<sm::notifiable*, ::uint32_t> _events;

			/* timer */
			sm::timer2<self> _timer;


			// -- private static methods --------------------------------------

			/* init */
			static auto _init(void) -> int {

				const int fd = ::inotify_init();

				if (fd == -1)
					throw sm::system_error("inotify_init");

				return fd;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inotify(void)
			: _fd{self::_init()} {
			}

			/* deleted copy constructor */
			inotify(const self&) = delete;

			/* move constructor */
			inotify(self&&) noexcept = default;

			/* destructor */
			~inotify(void) noexcept {

				// get end iterator
				const auto end = _map.end();

				// remove watches
				for (auto it = _map.begin(); it != end; ++it)
					static_cast<void>(::inotify_rm_watch(_fd, it->first));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* watch */
			auto watch(sm::notifiable& notifiable, const ::uint32_t& mask) -> void {

				// add watch
				const int wd = ::inotify_add_watch(_fd, notifiable.path(), mask);

				// check error
				if (wd == -1)
					throw sm::system_error("inotify_add_watch");

				// add to map
				_map[wd] = &notifiable;
			}


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return _fd;
			}

			/* events */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;


			auto handle_events(sm::taskmaster&) -> void;

	}; // class inotify

} // namespace sm

#endif // inotify_hpp
