#ifndef controller_hpp
#define controller_hpp

#include "taskmaster/server.hpp"
#include "taskmaster/client_manager.hpp"
#include "common/dispatch.hpp"
#include <unistd.h>


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

			/* clients */
			sm::client_manager _clients;

			/* is running */
			bool _running;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			controller(void) = default;


			auto run(void) -> void {

				_running = true;

				// daemonize

				// create server
				_server = sm::server{::in_port_t{4242}};

				_dispatch.subscribe(_server, sm::event{EPOLLIN});

				// poll
				while (_running == true) {

					// wait for events
					_dispatch.wait(*this);
					//::write(STDOUT_FILENO, ".", 1);
				}

				_dispatch.unsubscribe(_server);
			}


			// -- public accessors --------------------------------------------

			/* server */
			auto server(void) noexcept -> sm::server& {
				return _server;
			}

			/* monitor */
			auto monitor(void) noexcept -> sm::monitor<self>& {
				return _dispatch;
			}

			/* stop */
			auto stop(void) noexcept -> void {
				_running = false;
			}

			/* clients */
			auto clients(void) noexcept -> sm::client_manager& {
				return _clients;
			}

	}; // class controller

} // namespace sm

#endif // controller_hpp
