#ifndef TASKMASTER_SYSTEM_PIPE_HPP
#define TASKMASTER_SYSTEM_PIPE_HPP

#include <unistd.h>
#include <stdexcept>


// -- 4 2  N A M E S P A C E  -------------------------------------------------

namespace ft {


	// -- P I P E -------------------------------------------------------------

	class pipe final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ft::pipe;


			// -- private members ---------------------------------------------

			/* file descriptors */
			int _fds[2U];


			// -- private constants -------------------------------------------

			enum : unsigned int {
				RD = 0U,
				WR = 1U
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			pipe(void) {

				// create pipe
				if (::pipe(_fds) == -1)
					throw std::runtime_error("pipe failed");
			}

			/* deleted copy constructor */
			pipe(const self&) = delete;

			/* deleted move constructor */
			pipe(self&&) = delete;

			/* destructor */
			~pipe(void) noexcept {

				// close read end
				if (_fds[RD] != -1)
					::close(_fds[RD]);

				// close write end
				if (_fds[WR] != -1)
					::close(_fds[WR]);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public modifiers --------------------------------------------

			/* close read end */
			auto close_read(void) noexcept -> void {

				// check descriptor
				if (_fds[RD] == -1)
					return;

				// close read end
				::close(_fds[RD]);
				_fds[RD] = -1;
			}

			/* close write end */
			auto close_write(void) noexcept -> void {

				// check descriptor
				if (_fds[WR] == -1)
					return;

				// close write end
				::close(_fds[WR]);
				_fds[WR] = -1;
			}


			// -- public methods ----------------------------------------------

			/* read */
			template <typename T>
			auto read(T& data) -> void {
				const auto bytes = ::read(_fds[RD], &data, sizeof(T));

				if (bytes == -1)
					throw std::runtime_error("read failed");
			}

			/* write */
			template <typename T>
			auto write(const T& data) -> void {
				const auto bytes = ::write(_fds[WR], &data, sizeof(T));

				if (bytes == -1)
					throw std::runtime_error("write failed");
			}

	}; // class pipe

} // namespace ft

#endif // TASKMASTER_SYSTEM_PIPE_HPP
