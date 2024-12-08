#ifndef daemon_hpp
#define daemon_hpp

#include "process/process_id.hpp"
#include "fork.hpp"
#include "pipe.hpp"
#include "setsid.hpp"

#include <sys/types.h>
#include <sys/wait.h>

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


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
