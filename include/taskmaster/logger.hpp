#ifndef logger_hpp
#define logger_hpp

#include "common/resources/unique_fd.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <stdexcept>
#include <fcntl.h>

#include <sys/sendfile.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- L O G G E R ---------------------------------------------------------

	class logger final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::logger;


			// -- private members ---------------------------------------------

			/* file descriptor */
			int _fd;

			/* proc path */
			char _proc_path[PATH_MAX];


			// -- private static members --------------------------------------

			/* path */
			static constexpr const char* _path {
				"/var/log/taskmaster/taskmaster.log"
			};

			/* directories */
			static constexpr const char* _dirs[] {
				"/var",
				"/var/log",
				"/var/log/taskmaster"
			};


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			logger(void) noexcept
			: _fd{-1} {

				// create directories
				for (const auto& dir : _dirs) {

					struct ::stat st;

					// check if directory exists
					if (::stat(dir, &st) == -1) {

						if (errno != ENOENT) {
							perror("stat");
							return;
						}

						// create directory
						if (::mkdir(dir, 0755) == -1) {
							perror("mkdir");
							return;
						}
					}

					// check if directory
					if (!S_ISDIR(st.st_mode)) {
						perror("stat");
						return;
					}
				}

				// open file
				_fd = ::open(_path, O_CREAT | O_WRONLY, 0644);

				// check error
				if (_fd == -1) {
					perror("open");
					return;
				}

				// remove file
				if (::unlink(_path) == -1) {
					perror("unlink");
					return;
				}

				// get process id
				auto pid = ::getpid();

				snprintf(_proc_path, PATH_MAX, "/proc/%d/fd/%d", pid, _fd);

				// create symlink
				if (::symlink(_proc_path, _path) == -1) {
					perror("symlink");
					return;
				}

				::write(_fd, "hello, world!\n", 14);

				// get process path
				//__builtin_memcpy(_proc_path, "/proc/", 6U);

				//unsigned i = PATH_MAX;
				//
				//// convert pid to string
				//do {
				//
				//	_proc_path[--i] = static_cast<char>((pid % 10) ^ 48);
				//	pid /= 10;
				//
				//} while (pid != 0);
				//
				//__builtin_memcpy(_proc_path + 6U, _proc_path + i, PATH_MAX - i);

				//__builtin_memcpy(_proc_path + PATH_MAX - i, "/fd/1\0", 6U);

				//__builtin_memcpy(ptr, "/fd/1", 5U);

				printf("proc path: %s\n", _proc_path);


			}

			~logger(void) noexcept {

				if (_fd == -1)
					return;

				// remove symlink
				if (::unlink(_path) == -1)
					perror("unlink");

				// open proc file
				const int src = ::open(_proc_path, O_RDONLY);

				if (src == -1) {
					perror("src open");
					return;
				}

				// open log file
				const int dst = ::open(_path, O_WRONLY | O_CREAT, 0644);

				if (dst == -1) {
					perror("dst open");
					return;
				}

				// copy proc contents into file
				struct ::stat st;

				if (::fstat(src, &st) == -1) {
					perror("fstat");
					return;
				}

				printf("st_size: %ld\n", st.st_size);

				const auto bytes = ::sendfile(dst, src, nullptr, (size_t)st.st_size);

				if (bytes == -1) {
					perror("sendfile");
					return;
				}

				// close file
				static_cast<void>(::close(src));
				static_cast<void>(::close(dst));
			}




	}; // class logger

}

#include <sys/inotify.h>

namespace sm {

	class inotify final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::inotify;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			inotify(void)
			: _fd{::inotify_init()} {

				// check error
				if (_fd == -1)
					throw std::runtime_error("inotify_init failed");

			}

			auto watch(const char* path, const ::uint32_t& mask) -> sm::unique_fd {

				// add watch
				sm::unique_fd wd = ::inotify_add_watch(_fd, path, mask);

				// check error
				if (wd == -1)
					throw std::runtime_error("inotify_add_watch failed");

				return wd;
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

#endif // logger_hpp
