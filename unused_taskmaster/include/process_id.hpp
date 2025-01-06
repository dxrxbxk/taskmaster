#ifndef process_id_hpp
#define process_id_hpp

#include <sys/syscall.h>
#include <sys/wait.h>

#include "system/syscall.hpp"
#include "resources/unique_fd.hpp"
#include "process/wait_status.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O C E S S  I D --------------------------------------------------

	class process_id final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process_id;


			// -- private members ---------------------------------------------

			/* pid */
			::pid_t _pid;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_id(void) noexcept;

			/* pid constructor */
			process_id(const ::pid_t&) noexcept;

			/* copy constructor */
			process_id(const self&) noexcept = default;

			/* move constructor */
			process_id(self&&) noexcept = default;

			/* destructor */
			~process_id(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public convertion operators ---------------------------------

			/* pid_t conversion operator */
			operator const ::pid_t&(void) const noexcept;


			// -- public methods ----------------------------------------------

			/* open */
			auto open(void) const -> sm::unique_fd;

			/* wait */
			auto wait(const int& = 0) const -> sm::wait_status;

			/* kill */
			auto kill(const int& = SIGKILL) const -> void;

	}; // class process_id

} // namespace sm

#endif // process_id_hpp
