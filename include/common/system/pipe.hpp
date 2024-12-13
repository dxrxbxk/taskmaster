#ifndef pipe_hpp
#define pipe_hpp

#include "common/diagnostics/exception.hpp"
#include <unistd.h>
#include <fcntl.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class pipe final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::pipe;


			// -- private members ---------------------------------------------

			/* pipe file descriptors */
			int _fds[2U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			pipe(void);

			/* flags constructor */
			template <typename... Ts>
			pipe(Ts&&... flags)
			: sm::pipe{} {

				// get flags
				int ret = ::fcntl(_fds[1U], F_GETFD);

				// check for error
				if (ret == -1)
					throw sm::system_error("fcntl");

				// combine and
				ret |= (flags | ...);

				// set flags
				if (::fcntl(_fds[1U], F_SETFD, ret) == -1)
					throw sm::system_error("fcntl");
			}

			/* deleted copy constructor */
			pipe(const self&) = delete;

			/* move constructor */
			pipe(self&&) noexcept;

			/* destructor */
			~pipe(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self&;


			// -- public methods ----------------------------------------------

			/* close read */
			auto close_read(void) noexcept -> void;

			/* close write */
			auto close_write(void) noexcept -> void;

			/* read */
			template <typename T>
			auto read(T& buffer) -> void {

				const auto bytes = ::read(_fds[0U], &buffer, sizeof(buffer));

				if (bytes == -1)
					throw sm::system_error("read");
			}

			/* write */
			template <typename T>
			auto write(const T& buffer) -> void {

				const auto bytes = ::write(_fds[1U], &buffer, sizeof(buffer));

				if (bytes == -1)
					throw sm::system_error("write");
			}

	}; // class pipe

} // namespace sm

#endif // pipe_hpp
