#ifndef close_guard_hpp
#define close_guard_hpp

#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C L O S E  G U A R D ------------------------------------------------

	template <unsigned N = 1U>
	class close_guard final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::close_guard<N>;


			// -- private members ---------------------------------------------

			/* file descriptor */
			int _fd[N];


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			close_guard(void) = delete;

			/* descriptors constructor */
			template <typename... Ts>
			close_guard(const Ts&... fds) noexcept
			: _fd{fds...} {
			}

			/* deleted copy constructor */
			close_guard(const self&) = delete;

			/* deleted move constructor */
			close_guard(self&&) = delete;

			/* destructor */
			~close_guard(void) noexcept {

				// close file descriptors
				for (unsigned i = 0U; i < N; ++i)
					static_cast<void>(::close(_fd[i]));
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator const int&(void) const noexcept requires (N == 1U) {
				return _fd[0U];
			}


			// -- public operators --------------------------------------------

			/* subscript operator */
			auto operator[](const unsigned& i) const noexcept -> const int& requires (N > 1U) {
				return _fd[i];
			}

	}; // class close_guard

} // namespace sm

#endif // close_guard_hpp
