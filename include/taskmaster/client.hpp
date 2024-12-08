#ifndef client_hpp
#define client_hpp

#include "common/network/socket.hpp"
#include "common/dispatch.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* controller */
	class controller;


	// -- C L I E N T ---------------------------------------------------------

	class client final : public sm::listener<sm::controller> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::client;


			// -- private members ---------------------------------------------

			/* socket */
			sm::socket _socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client(void) noexcept = default;

			/* socket constructor */
			client(sm::socket&&) noexcept;

			/* deleted copy constructor */
			client(const self&) = delete;

			/* move constructor */
			client(self&&) noexcept = default;

			/* destructor */
			~client(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(sm::controller&, const sm::event&) -> void override;

	}; // class client

} // namespace sm

#endif // client_hpp
