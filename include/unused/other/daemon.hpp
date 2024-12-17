#ifndef daemon_hpp
#define daemon_hpp


#include "system/fork.hpp"
#include "system/setsid.hpp"
#include "diagnostics/exception.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- D A E M O N ---------------------------------------------------------

	class daemon final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::daemon;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			daemon(void) noexcept = default;

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

			/* daemonize */
			auto daemonize(void) {

				try {


				}

				catch (const sm::exception& e) {

				}

			}


	}; // class daemon


	/* daemonize */
	template <typename T>
	auto daemonize(T&& function) -> void {

		// check for root
		if (::getuid() != 0)
			throw sm::system_error("daemonize: must be root");

		// fork
		auto pid = sm::fork();

		// parent process
		if (pid > 0)
			return;

		// create new session
		sm::setsid();

		// fork
		pid = sm::fork();

		// second parent process
		if (pid > 0)
			return;

		// close standard descriptors
		static_cast<void>(::close(STDIN_FILENO));
		static_cast<void>(::close(STDOUT_FILENO));
		static_cast<void>(::close(STDERR_FILENO));

		// change directory
		static_cast<void>(::chdir("/"));

		// change umask
		::umask(0);

		// run function
		function();
	}


} // namespace sm

#endif // daemon_hpp
