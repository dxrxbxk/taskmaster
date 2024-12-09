#ifndef server_hpp
#define server_hpp

#include "io_event.hpp"
#include "common/dispatch.hpp"
#include "common/network/socket.hpp"
#include "common/network/addr.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- S E R V E R ---------------------------------------------------------

	class server final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::server;


			// -- private members ---------------------------------------------

			/* socket */
			sm::socket _socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			server(void) noexcept = default;

			/* port constructor */
			server(const ::in_port_t&);

			/* deleted copy constructor */
			server(const self&) = delete;

			/* move constructor */
			server(self&&) noexcept = default;

			/* destructor */
			~server(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&) -> void override;

	}; // class server

} // namespace sm

#endif // server_hpp
