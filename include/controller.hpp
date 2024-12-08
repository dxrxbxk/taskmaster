#ifndef controller_hpp
#define controller_hpp

#include "network/server.hpp"
#include "dispatch.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C O N T R O L L E R -------------------------------------------------

	class controller final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::controller;


			// -- private members ---------------------------------------------

			/* server */
			sm::server _server;

			/* dispatcher */
			sm::monitor<self> _dispatch;

			/* is running */
			bool _running;


		public:


			auto run(void) -> void {

				// daemonize

				// create server
				_server = sm::server{::in_port_t{4242}};

				_dispatch.subscribe(_server, sm::event{EPOLLIN});

				// poll
				while (_running == true) {

					// wait for events
					_dispatch.wait(*this);
				}

				_dispatch.unsubscribe(_server);
			}


			// -- public accessors --------------------------------------------

			/* server */
			auto server(void) noexcept -> sm::server& {
				return _server;
			}

			/* const server */
			auto server(void) const noexcept -> const sm::server& {
				return _server;
			}

			/* monitor */
			auto monitor(void) noexcept -> sm::monitor<self>& {
				return _dispatch;
			}

			/* const monitor */
			auto monitor(void) const noexcept -> const sm::monitor<self>& {
				return _dispatch;
			}

			/* stop */
			auto stop(void) noexcept -> void {
				_running = false;
			}

	}; // class controller

} // namespace sm

#endif // controller_hpp
