#ifndef process_event_hpp
#define process_event_hpp

#include "io_event.hpp"

#include "resources/unique_fd.hpp"
#include "diagnostics/exception.hpp"
#include "process/process_id.hpp"

#include <iostream>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O C E S S -------------------------------------------------------

	class process final : public sm::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process;


			// -- private members ---------------------------------------------

			/* process id */
			sm::process_id _pid;

			/* unique fd */
			sm::unique_fd _fd;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process(void) noexcept = default;

			/* pid constructor */
			process(const sm::process_id& pid)
			: _pid{pid}, _fd{pid.open()} {
			}

			/* deleted copy constructor */
			process(const self&) = delete;

			/* move constructor */
			process(self&&) noexcept = default;

			/* destructor */
			~process(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public override ---------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return static_cast<int>(_fd);
			}

			/* on event */
			auto on_event(const ::uint32_t& events) -> void override {

				std::cout << "process on event: " << events << std::endl;
				std::cout << "pid: " << _pid << std::endl;

				int pidfd = _fd;
				::pid_t pid = _pid;

				siginfo_t info;
				const auto status = ::waitid(P_PIDFD, (__id_t)pidfd, &info, WEXITED);

				if (status == -1) {
					if (errno == ECHILD) {
						std::cerr << "Process already collected or does not exist." << std::endl;
					} else {
						throw sm::system_error("waitid");
					}
					exit(EXIT_FAILURE);
				}


				if (info.si_code == CLD_EXITED) {
					printf("Process exited normally with status %d\n", info.si_status);
				} else if (info.si_code == CLD_KILLED || info.si_code == CLD_DUMPED) {
					printf("Process was terminated by signal %d\n", info.si_status);
				}

				exit(EXIT_SUCCESS);
				//_pid.wait();
				//if (events & EPOLLIN) {
				//
				//}
			}

	}; // class process_event

} // namespace sm

#endif // process_event_hpp
