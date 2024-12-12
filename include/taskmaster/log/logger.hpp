#ifndef logger_hpp
#define logger_hpp

#include "common/resources/unique_fd.hpp"
#include "taskmaster/log/timestamp.hpp"
#include "common/diagnostics/exception.hpp"
#include "common/string/strlen.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

#include <sys/sendfile.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- L O G G E R ---------------------------------------------------------

	class logger final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::logger;

			/* log function */
			using log_func = auto (self::*)(const char*, const char*) -> void;


			// -- private members ---------------------------------------------

			/* file descriptor */
			//sm::unique_fd _fd;

			/* functions */
			log_func _function;


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
			: _function{&self::_log_standard} {
			}

			/* deleted copy constructor */
			logger(const self&) = delete;

			/* deleted move constructor */
			logger(self&&) = delete;

			/* destructor */
			~logger(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}



			auto _log_standard(const char* level, const char* message) -> void {

				const auto len = sm::strlen(level);

				// get timestamp
				sm::timestamp ts;

				(void)::write(STDOUT_FILENO, ts.data(), ts.size());
				(void)::write(STDOUT_FILENO, " ", 1);
				(void)::write(STDOUT_FILENO, level, len);
				(void)::write(STDOUT_FILENO, ": ", 2);
				(void)::write(STDOUT_FILENO, message, ::strlen(message));
				(void)::write(STDOUT_FILENO, "\n", 1);
			}

			auto _log_daemon(const char* level, const char* message) -> void {

				// open log file
				int fd = ::open(_path, O_WRONLY | O_CREAT | O_APPEND, 0644);

				if (fd == -1) {

					if (errno != ENOENT)
						return;

					// check directories
					for (const auto& dir : _dirs) {

						struct ::stat st;

						// check if directory exists
						if (::stat(dir, &st) == -1) {

							if (errno != ENOENT)
								return;

							// create directory
							if (::mkdir(dir, 0755) == -1)
								return;
						}

						// check if directory
						if (!S_ISDIR(st.st_mode))
							return;
					}

					// open file
					fd = ::open(_path, O_CREAT | O_WRONLY | O_APPEND, 0644);

					// check error
					if (fd == -1)
						return;

				}

				const auto len = sm::strlen(level);

				// get timestamp
				sm::timestamp ts;

				::write(fd, ts.data(), ts.size());
				::write(fd, " ", 1);
				::write(fd, level, len);
				::write(fd, ": ", 2);
				::write(fd, message, ::strlen(message));
				::write(fd, "\n", 1);

				// close file
				static_cast<void>(::close(fd));
			}


		public:

			// -- public static methods ---------------------------------------

			/* log to standard */
			static auto log_to_standard(void) -> void {
				self::_shared()._function = &self::_log_standard;
			}

			/* log to daemon */
			static auto log_to_daemon(void) -> void {
				self::_shared()._function = &self::_log_daemon;
			}


			/* call */
			auto _call(const char* level, const char* message) -> void {
				(this->*_function)(level, message);
			}

			/* info */
			static auto info(const char* message) -> void {
				self::_shared()._call("\x1b[32mINFO\x1b[0m", message);
			}

			/* warn */
			static auto warn(const char* message) -> void {
				self::_shared()._call("\x1b[33mWARN\x1b[0m", message);
			}

			/* debug */
			static auto debug(const char* message) -> void {
				self::_shared()._call("\x1b[34mDEBG\x1b[0m", message);
			}

			/* error */
			static auto error(const char* message) -> void {
				self::_shared()._call("\x1b[31mERRO\x1b[0m", message);
			}

			/* signal */
			static auto signal(const char* message) -> void {
				self::_shared()._call("\x1b[35mSIGN\x1b[0m", message);
			}

	}; // class logger

} // namespace sm

#endif // logger_hpp






				// create directories
				//for (const auto& dir : _dirs) {
				//
				//	struct ::stat st;
				//
				//	// check if directory exists
				//	if (::stat(dir, &st) == -1) {
				//
				//		if (errno != ENOENT) {
				//			//perror("stat");
				//			return;
				//		}
				//
				//		// create directory
				//		if (::mkdir(dir, 0755) == -1) {
				//			//perror("mkdir");
				//			return;
				//		}
				//	}
				//
				//	// check if directory
				//	if (!S_ISDIR(st.st_mode)) {
				//		//perror("stat");
				//		return;
				//	}
				//}
				//
				//// open file
				//_fd = ::open(_path, O_CREAT | O_WRONLY, 0644);
				//
				//// check error
				//if (_fd == -1) {
				//	//perror("open");
				//	return;
				//}
				//
				//// remove file
				//if (::unlink(_path) == -1) {
				//	//perror("unlink");
				//	return;
				//}
				//
				//// get process id
				//auto pid = ::getpid();
				//
				////snprintf(_proc_path, PATH_MAX, "/proc/%d/fd/%d", pid, _fd);
				//
				//// create symlink
				//if (::symlink(_proc_path, _path) == -1) {
				//	//perror("symlink");
				//	return;
				//}
				//
				//::write(_fd, "hello, world!\n", 14);

				// ---------------------------------

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

				//printf("proc path: %s\n", _proc_path);


				//if (_fd == -1)
				//	return;
				//
				//// remove symlink
				//if (::unlink(_path) == -1)
				//	perror("unlink");
				//
				//// open proc file
				//const int src = ::open(_proc_path, O_RDONLY);
				//
				//if (src == -1) {
				//	perror("src open");
				//	return;
				//}
				//
				//// open log file
				//const int dst = ::open(_path, O_WRONLY | O_CREAT, 0644);
				//
				//if (dst == -1) {
				//	perror("dst open");
				//	return;
				//}
				//
				//// copy proc contents into file
				//struct ::stat st;
				//
				//if (::fstat(src, &st) == -1) {
				//	perror("fstat");
				//	return;
				//}
				//
				//printf("st_size: %ld\n", st.st_size);
				//
				//const auto bytes = ::sendfile(dst, src, nullptr, (size_t)st.st_size);
				//
				//if (bytes == -1) {
				//	perror("sendfile");
				//	return;
				//}
				//
				//// close file
				//static_cast<void>(::close(src));
				//static_cast<void>(::close(dst));
