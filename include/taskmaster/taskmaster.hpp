#ifndef taskmaster_hpp
#define taskmaster_hpp

#include "taskmaster/network/server.hpp"
#include "taskmaster/network/client_manager.hpp"
#include "common/dispatch.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- T A S K M A S T E R -------------------------------------------------

	class taskmaster final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::taskmaster;


			// -- private members ---------------------------------------------

			/* server */
			sm::server _server;

			/* dispatcher */
			sm::monitor _monitor;

			/* clients */
			sm::client_manager _clients;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> self&;

			/* daemonize */
			static auto _daemonize(void) -> bool;

			auto _run(void) -> void;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			taskmaster(void);

			/* deleted copy constructor */
			taskmaster(const self&) = delete;

			/* deleted move constructor */
			taskmaster(self&&) = delete;

			/* destructor */
			~taskmaster(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;



		public:

			// -- public static methods ---------------------------------------

			/* run */
			static auto run(void) -> void;

			/* server */
			static auto server(void) noexcept -> sm::server&;

			/* monitor */
			static auto monitor(void) noexcept -> sm::monitor&;

			/* stop */
			static auto stop(void) noexcept -> void;

			/* clients */
			static auto clients(void) noexcept -> sm::client_manager&;

	}; // class taskmaster

} // namespace sm

#endif // taskmaster_hpp
