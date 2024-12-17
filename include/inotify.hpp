#ifndef inotify_hpp
#define inotify_hpp

#include "events/monitor.hpp"
#include "resources/unique_fd.hpp"
#include "diagnostics/exception.hpp"
#include <sys/inotify.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- I N O T I F Y -------------------------------------------------------

	class inotify final : public sm::monitor {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::inotify;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;


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
			~inotify(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			auto watch(const char* path, const ::uint32_t& mask) -> sm::unique_fd {

				// add watch
				sm::unique_fd wd = ::inotify_add_watch(_fd, path, mask);

				// check error
				if (wd == -1)
					throw sm::system_error("inotify_add_watch");

				return wd;
			}


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return _fd;
			}

			/* events */
			auto events(void) -> void override {

				// buffer
				char buffer[4096U];

				// read events
				const auto bytes = ::read(_fd, buffer, 4096U);

				// check error
				if (bytes == -1)
					throw sm::system_error("read");

				// loop over events
				for (unsigned i = 0U; i < bytes; ) {

					// event
					const auto* event = reinterpret_cast<struct ::inotify_event*>(buffer + i);

					// next event
					i += sizeof(struct ::inotify_event) + event->len;

					// check mask
					if (event->mask & IN_CREATE)
						printf("file created\n");

					if (event->mask & IN_DELETE)
						printf("file deleted\n");
				}
			}

	}; // class inotify


				// buffer
			//	char buffer[4096];
			//
			//	// read events
			//	const auto bytes = ::read(_fd, buffer, 4096);
			//
			//	// check error
			//	if (bytes == -1) {
			//		perror("read");
			//		return;
			//	}
			//
			//	// loop over events
			//	for (unsigned i = 0U; i < bytes; ) {
			//
			//		// event
			//		const auto* event = reinterpret_cast<struct ::inotify_event*>(buffer + i);
			//
			//		// next event
			//		i += sizeof(struct ::inotify_event) + event->len;
			//
			//		// check mask
			//		if (event->mask & IN_CREATE)
			//			printf("file created\n");
			//
			//		if (event->mask & IN_DELETE)
			//			printf("file deleted\n");
			//	}
			//
			//}
			//

} // namespace sm

#endif // inotify_hpp
