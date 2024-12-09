#ifndef daemon_hpp
#define daemon_hpp


#include "process/process_id.hpp"

#include "common/system/fork.hpp"
#include "common/system/setsid.hpp"
#include "common/diagnostics/exception.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	/* close descriptors */
	inline auto close_descriptors(void) noexcept -> void {

		// close standard descriptors
		static_cast<void>(::close(STDIN_FILENO));
		static_cast<void>(::close(STDOUT_FILENO));
		static_cast<void>(::close(STDERR_FILENO));
	}


	/* daemonize */
	template <typename T>
	auto daemonize(void) -> void {

		// check for root
		if (::getuid() != 0)
			throw sm::system_error("daemonize: must be root");

		// fork
		auto pid = sm::fork();

		// parent process
		if (pid > 0)
			return;

		// intermediate process

		// create new session
		sm::setsid();

		// fork
		pid = sm::fork();

		// second parent process
		if (pid > 0)
			return;

		// child process

		//sm::unique_fd log{::open("/tmp/daemon.log", O_WRONLY | O_CREAT | O_APPEND, 0644)};

		// dup to /dev/null
		//dup2(STDIN_FILENO, log);

		// close descriptors
		close_descriptors();

		// change directory
		static_cast<void>(::chdir("/"));

		// change umask
		::umask(0);

		// run daemon
		T{};
	}



	// -- D A E M O N ---------------------------------------------------------

	class daemon final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::daemon;


			// -- private members ---------------------------------------------

			/* process id */
			sm::process_id _pid;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			daemon(void)
			: _pid{} {
			}

			/* copy constructor */
			daemon(const self&) = default;

			/* move constructor */
			daemon(self&&) noexcept = default;

			/* destructor */
			~daemon(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;



			// -- public methods ----------------------------------------------

			/* run */
			auto run(char** argv) -> sm::process_id {

				{
					auto pid = sm::fork();

					if (pid > 0) {
						return sm::process_id{pid};
					}
					else {

						// dup2 to /dev/null


						::execvp(argv[0], argv);
						exit(EXIT_SUCCESS);
					}
				}

				/*
				ft::pipe pipe;

				auto pid = ft::fork();

				if (pid > 0) {

					// -- parent ----------------------------------------------

					// close write end
					pipe.close_write();

					::pid_t daemon_pid = 0;

					pipe.read(daemon_pid);
					pipe.close_read();

					::waitpid(pid, nullptr, 0);

					std::cout << "daemon pid: " << daemon_pid << std::endl;

					return sm::process_id{daemon_pid};
				}
				else {

					// -- intermediate ----------------------------------------

					// new session
					sm::setsid();

					// close read end
					pipe.close_read();

					pid = ft::fork();

					if (pid > 0) {

						// -- second parent -----------------------------------

						// write daemon pid
						pipe.write(pid);
						pipe.close_write();

						// exit
						exit(EXIT_SUCCESS);

					}
					else {

						// -- child -------------------------------------------

						// close write end
						pipe.close_write();

						::execvp(argv[0], argv);

						exit(EXIT_SUCCESS);
					}

				}
				*/

			}


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const -> bool {
				// done
				return true;
			}

			/* process id */
			auto process_id(void) const -> const sm::process_id& {
				return _pid;
			}


	}; // class daemon

} // namespace sm

#endif // daemon_hpp
