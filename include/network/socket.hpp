#ifndef socket_hpp
#define socket_hpp

#include "unique_fd.hpp"
#include <sys/socket.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* addr */
	class addr;


	// -- S O C K E T ---------------------------------------------------------

	class socket : public sm::unique_fd {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			socket(void) noexcept = default;

			/* socket constructor */
			socket(const int&, const int&, const int& = 0);

			/* deleted copy constructor */
			socket(const self&) = delete;

			/* move constructor */
			socket(self&&) noexcept = default;

			/* destructor */
			~socket(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public conversion operators ---------------------------------

			/* int conversion operator */
			operator const int&(void) const noexcept;


			// -- public methods ----------------------------------------------

			/* bind */
			auto bind(const sm::addr&) const -> void;

			/* listen */
			auto listen(int = SOMAXCONN) const -> void;

			/* accept */
			auto accept(sm::addr&) const -> self;

			/* accept */
			auto accept(void) const -> self;

			/* connect */
			auto connect(const sm::addr&) const -> void;

			/* non blocking */
			auto non_blocking(void) const -> void;

			/* shutdown */
			auto shutdown(void) const -> void;

			/* reuse address */
			auto reuse_address(void) const -> void;

	}; // class socket

} // namespace sm

#endif // socket_hpp
