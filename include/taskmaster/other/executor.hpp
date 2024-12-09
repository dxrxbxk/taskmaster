#ifndef executor_hpp
#define executor_hpp

#include "common/system/fork.hpp"
#include "common/system/open.hpp"
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E X E C U T O R -----------------------------------------------------

	class executor final {

		public:

			auto exec(void) -> void {

				// fork
				const auto pid = sm::fork();

				// parent process
				if (pid != 0)
					return;

				// child process

				// redirect stdout
				const int fd = sm::open("./log.txt", O_CREAT | O_WRONLY, 0644);

				auto ret = ::dup2(fd, STDOUT_FILENO);
			}

	}; // class executor



} // namespace sm

#endif // executor_hpp
