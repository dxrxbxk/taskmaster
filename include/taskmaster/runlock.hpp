#ifndef runlock_hpp
#define runlock_hpp

#include "common/resources/unique_fd.hpp"
#include "common/guards/lock_guard.hpp"
#include "common/guards/immutable_guard.hpp"
#include "common/system/open.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R U N L O C K -------------------------------------------------------

	class runlock final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::runlock;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* lock guard */
			sm::lock_guard _guard;

			/* immutable guard */
			sm::immutable_guard _immutable;


			// -- private statis methods --------------------------------------

			/* open */
			static auto _open(void) -> int {

				// create path if not exists
				// '/var/lock/taskmaster/'


				// create file
				// '/var/lock/taskmaster/taskmaster.lock'

				// to be implemented...

				return sm::open("./taskmaster.lock", O_CREAT, 0000);
			}

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runlock(void)
			: _fd{self::_open()},
			  _guard{_fd},
			  _immutable{_fd} {
			}

			/* deleted copy constructor */
			runlock(const self&) = delete;

			/* deleted move constructor */
			runlock(self&&) = delete;

			/* destructor */
			~runlock(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;



		private:


			auto list_processes(void) -> void {

				/*
				struct ::dirent* entry;

				sm::unique_dir proc{"/proc"};

				constexpr char line[] = "Name:\ttaskmaster\n";
				constexpr auto length = sizeof(line) - 1U;

				const ::pid_t process_id = ::getpid();

				start:
				// loop over entries
				while ((entry = proc.read()) != nullptr) {

					// skip non-directories
					if (entry->d_type != DT_DIR)
						continue;


					// declare pid
					::pid_t pid = 0;

					//// check for numeric directory name
					//for (unsigned i = 0U; i < sizeof(entry->d_name)
					//		|| entry->d_name[i] != '\0'; ++i) {
					//
					//	pid *= 10;
					//	pid += static_cast<::pid_t>(entry->d_name[i] - '0');
					//
					//	if (entry->d_name[i] < '0'
					//	 || entry->d_name[i] > '9') {
					//		goto start;
					//	}
					//}


					// vérifier si le nom du répertoire est un nombre (pid)
					if (not isdigit(entry->d_name[0U])) {
						continue;
					}

					// vérifier si le pid est le notre
					if (::atoi(entry->d_name) == pid) {
						continue;
					}

					char path[PATH_MAX];

					snprintf(path, PATH_MAX, "/proc/%s/status", entry->d_name);

					const sm::unique_fd fd = sm::open(path, O_RDONLY);

					sm::reader<length> reader;

					reader.read(fd);

					if (reader.size() == length
					&& strncmp(reader.data(), line, reader.size()) == 0)
						throw sm::runtime_error("taskmaster is already running");
				}
						*/
			}

		}; // class runlock

} // namespace sm

#endif // runlock_hpp
