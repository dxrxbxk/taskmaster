/* ************************************************************************* */
/*                                                                           */
/*            ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖              */
/*              █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌             */
/*              █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖             */
/*              █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌             */
/*                                                                           */
/* ************************************************************************* */

#ifndef TASKMASTER_CORE_DAEMON_HPP
#define TASKMASTER_CORE_DAEMON_HPP

#include "system/fork.hpp"
#include "system/pipe.hpp"

#include <sys/types.h>
#include <sys/wait.h>


// -- 4 2  N A M E S P A C E  -------------------------------------------------

namespace ft {


	// -- D A E M O N ---------------------------------------------------------

	class daemon final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ft::daemon;


			// -- private members ---------------------------------------------

			/* process id */
			::pid_t _pid;

			/* arguments */
			//tsk::exec_args _args;



		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			daemon(void)
			: _pid{-1}/*, _args{}*/ {
			}

			/* command constructor */
			//daemon(const std::vector<std::string>& args)
			//: _args{} {
			//
			//	for (const auto& arg : args)
			//		_args.push(arg);
			//}

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
			auto run(void) -> void {

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

					//std::cout << "daemon pid: " << daemon_pid << std::endl;
				}
				else {

					// -- intermediate ----------------------------------------

					// new session
					//sys::setsid();

					// close read end
					pipe.close_read();

					pid = ft::fork();

					if (pid > 0) {

						// -- second parent -----------------------------------

						// write daemon pid
						pipe.write(pid);
						pipe.close_write();

						// exit
						return;

					}
					else {

						// -- child -------------------------------------------

						// close write end
						pipe.close_write();
					}

				}

			}


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const -> bool {
				// done
				return true;
			}


	};

} // namespace ft

#endif // TASKMASTER_CORE_DAEMON_HPP
