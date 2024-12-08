#ifndef controller_hpp
#define controller_hpp

#include "taskmaster/server.hpp"
#include "taskmaster/client_manager.hpp"
#include "common/dispatch.hpp"
#include "common/signal.hpp"
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
			sm::monitor<self> _monitor;

			/* clients */
			sm::client_manager _clients;

			/* is running */
			bool _running;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			controller(void)
			: _server{},
			  _monitor{},
			  _clients{},
			  _running{true} {

				// subscribe to signals
				_monitor.subscribe(sm::signal::shared(), sm::event{EPOLLIN});
			}


			auto run(void) -> void {

				// daemonize

				// create server
				_server = sm::server{::in_port_t{4242}};

				_monitor.subscribe(_server, sm::event{EPOLLIN});

				// poll
				while (_running == true) {

					// wait for events
					_monitor.wait(*this);
					//::write(STDOUT_FILENO, ".", 1);
				}

				_monitor.unsubscribe(_server);
			}


			// -- public accessors --------------------------------------------

			/* server */
			auto server(void) noexcept -> sm::server& {
				return _server;
			}

			/* monitor */
			auto monitor(void) noexcept -> sm::monitor<self>& {
				return _monitor;
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
