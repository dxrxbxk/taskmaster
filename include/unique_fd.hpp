#ifndef unique_fd_hpp
#define unique_fd_hpp


// -- S M  N A M E S P A C E  -------------------------------------------------

namespace sm {


	// -- U N I Q U E  F D ----------------------------------------------------

	class unique_fd {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::unique_fd;


		protected:

			// -- protected members -------------------------------------------

			/* file descriptor */
			int _fd;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unique_fd(void) noexcept;

			/* int constructor */
			unique_fd(const int&) noexcept;

			/* deleted copy constructor */
			unique_fd(const self&) = delete;

			/* move constructor */
			unique_fd(self&&) noexcept;

			/* destructor */
			~unique_fd(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self&;


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator const int&(void) const noexcept;

			/* bool conversion operator */
			explicit operator bool(void) const noexcept;


			// -- public operators --------------------------------------------

			/* not operator */
			auto operator!(void) const noexcept -> bool;


			// -- public methods ----------------------------------------------

			/* close */
			auto close(void) -> void;

	}; // class unique_fd

} // namespace sm

#endif // unique_fd_hpp
