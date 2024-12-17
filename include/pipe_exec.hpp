#ifndef pipe_exec_hpp
#define pipe_exec_hpp

#include "diagnostics/exception.hpp"
#include <unistd.h>
#include <fcntl.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class pipe_exec final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::pipe_exec;


			// -- private members ---------------------------------------------

			/* pipe file descriptors */
			int _fds[2U];

			/* buffer */
			char _buffer[4096U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			pipe_exec(void);

			/* deleted copy constructor */
			pipe_exec(const self&) = delete;

			/* deleted move constructor */
			pipe_exec(self&&) = delete;

			/* destructor */
			~pipe_exec(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* read */
			auto read(void) -> sm::isize;

			/* write */
			auto write(const char*) -> void;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) const noexcept -> const char*;

	}; // class pipe

} // namespace sm

#endif // pipe_hpp
