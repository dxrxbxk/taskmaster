#ifndef reader_hpp
#define reader_hpp

#include "types.hpp"
#include "diagnostics/exception.hpp"

#include <unistd.h>
#include <sys/socket.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R E A D E R ---------------------------------------------------------

	template <unsigned int N>
	class reader final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::reader<N>;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[N];

			/* size */
			sm::usize _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			reader(void) noexcept
			: /* _buffer{} uninitialized */ _size{0U} {
			}

			/* copy constructor */
			reader(const self&) noexcept = default;

			/* move constructor */
			reader(self&&) noexcept = default;

			/* destructor */
			~reader(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) const noexcept -> const char* {
				return _buffer;
			}

			/* size */
			auto size(void) const noexcept -> const sm::usize& {
				return _size;
			}

			/* operator[] */
			auto operator[](const sm::usize& i) noexcept -> char& {
				return _buffer[i];
			}

			/* operator[] */
			auto operator[](const sm::usize& i) const noexcept -> const char& {
				return _buffer[i];
			}

			/* eof */
			auto eof(void) const noexcept -> bool {
				return _size == 0U;
			}


			// -- public methods ----------------------------------------------

			/* read */
			auto read(const int& fd) -> void {

				const auto bytes = ::read(fd, _buffer, N);

				if (bytes == -1) {

					_size = 0U;

					//if (errno != EINTR)
					throw sm::system_error("read");

					return;
				}

				_size = static_cast<sm::usize>(bytes);
			}

			/* receive */
			auto receive(const int& fd) -> void {

				const auto bytes = ::recv(fd, _buffer, N, 0);

				if (bytes == -1)
					throw sm::system_error("recv");

				_size = static_cast<sm::usize>(bytes);
			}

	}; // class reader

} // namespace sm

#endif // reader_hpp
