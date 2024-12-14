#ifndef taskmaster_hpp
#define taskmaster_hpp

#include "taskmaster/network/server.hpp"
#include "taskmaster/network/client_manager.hpp"
#include "taskmaster/program_manager.hpp"
#include "taskmaster/events/monitor.hpp"
#include "taskmaster/runlock.hpp"
#include "taskmaster/config.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* options */
	class options;


	// -- T A S K M A S T E R -------------------------------------------------

	class taskmaster final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::taskmaster;


			// -- private members ---------------------------------------------

			/* runlock */
			sm::runlock _runlock;

			/* config */
			sm::config _config;

			/* running */
			bool _running;

			/* server */
			sm::server _server;

			/* dispatcher */
			sm::monitor _monitor;

			/* clients */
			sm::client_manager _clients;

			/* program manager */
			sm::program_manager _programs;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(const sm::options&) -> self&;

			/* launch */
			static auto _launch(const sm::options&) -> self;

			/* daemonize */
			static auto _daemonize(void) -> bool;



			// -- private lifecycle -------------------------------------------

			/* deleted default constructor */
			taskmaster(void) = delete;

			/* options constructor */
			taskmaster(const sm::options&);

			/* deleted copy constructor */
			taskmaster(const self&) = delete;

			/* deleted move constructor */
			taskmaster(self&&) = delete;

			/* destructor */
			~taskmaster(void) noexcept;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private methods ---------------------------------------------

			/* run */
			auto _run(void) -> void;


		public:

			// -- public static methods ---------------------------------------

			/* run */
			static auto run(const sm::options&) -> void;

			/* server */
			auto server(void) noexcept -> sm::server&;

			/* monitor */
			auto monitor(void) noexcept -> sm::monitor&;

			/* stop */
			auto stop(void) noexcept -> void;

			/* clients */
			auto clients(void) noexcept -> sm::client_manager&;

	}; // class taskmaster

} // namespace sm

#endif // taskmaster_hpp
