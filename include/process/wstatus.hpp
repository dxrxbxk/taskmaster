#ifndef sm_process_wstatus
#define sm_process_wstatus

#include <sys/wait.h>


// -- S E R V I C E  M A N A G E R --------------------------------------------

namespace sm {



	// -- W S T A T U S -------------------------------------------------------

	class wstatus final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::wstatus;


			// -- private members ---------------------------------------------

			/* status */
			int _status;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			wstatus(void) noexcept
			: _status{0} {
			}

			/* status constructor */
			wstatus(const int& status) noexcept
			: _status{status} {
			}

			/* copy constructor */
			wstatus(const self&) noexcept = default;

			/* move constructor */
			wstatus(self&&) noexcept = default;

			/* destructor */
			~wstatus(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public conversion operators ---------------------------------

			/* int& conversion operator */
			operator int&(void) noexcept {
				return _status;
			}

			/* const int& conversion operator */
			operator const int&(void) const noexcept {
				return _status;
			}


			// -- public accessors --------------------------------------------

			/* is terminated normally */
			auto is_terminated_normally(void) const noexcept -> bool {
				// returns true if the child terminated normally,
				// that is, by calling exit(3) or _exit(2), or by returning from main().
				return WIFEXITED(_status);
			}

			/* exit status */
			auto exit_status(void) const noexcept -> int {
				// returns the exit status of the child.
				// This consists of the least significant 8 bits of the status argument that the child specified in a call to exit(3)
				// or _exit(2) or as the argument for a return statement in main().
				// This macro should be employed only if WIFEXITED returned true.
				return WEXITSTATUS(_status);
			}

			/* is signaled */
			auto is_signaled(void) const noexcept -> bool {
				// returns true if the child process was terminated by a signal.
				return WIFSIGNALED(_status);
			}

			/* signal */
			auto signal(void) const noexcept -> int {
				// returns the number of the signal that caused the child process to terminate.
				// This macro should be employed only if WIFSIGNALED returned true.
				return WTERMSIG(_status);
			}

			/* is core dumped */
			auto is_core_dumped(void) const noexcept -> bool {
				// returns true if the child produced a core dump (see core(5)).
				// This macro should be employed only if WIFSIGNALED returned true.
				return WCOREDUMP(_status);
			}

			/* is stopped */
			auto is_stopped(void) const noexcept -> bool {
				// returns  true if the child process was stopped by delivery of a signal;
				// this is possible only if the call was done using WUNTRACED or when the child is being traced (see ptrace(2)).
				return WIFSTOPPED(_status);
			}

			/* stop signal */
			auto stop_signal(void) const noexcept -> int {
				// returns the number of the signal which caused the child to stop.
				// This macro should be employed only if WIFSTOPPED returned true.
				return WSTOPSIG(_status);
			}

			/* is continued */
			auto is_continued(void) const noexcept -> bool {
				// (since Linux 2.6.10) returns true if the child process was resumed by delivery of SIGCONT.
				return WIFCONTINUED(_status);
			}

	}; // class wstatus

} // namespace sm

#endif // sm_process_wstatus
