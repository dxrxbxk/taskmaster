#ifndef taskmaster_hpp
#define taskmaster_hpp

#include "program_manager.hpp"
#include "events/monitor.hpp"
#include "config/config.hpp"
#include "executor.hpp"


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

			/* config */
			sm::config _config;

			/* running */
			bool _running;

			/* dispatcher */
			sm::monitor _monitor;

			/* program manager */
			sm::program_manager _programs;

			/* executor */
			sm::executor _executor;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(const sm::options&) -> self&;


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


			// -- public accessors --------------------------------------------

			/* monitor */
			auto monitor(void) noexcept -> sm::monitor&;

			/* programs */
			auto programs(void) noexcept -> sm::program_manager&;

			/* executor */
			auto executor(void) noexcept -> sm::executor&;

			/* is running */
			auto is_running(void) const noexcept -> bool;


			// -- public modifiers --------------------------------------------

			/* stop */
			auto stop(void) noexcept -> void;

			/* reload */
			auto reload(void) -> void;

	}; // class taskmaster

} // namespace sm

#endif // taskmaster_hpp
