#ifndef signal_hpp
#define signal_hpp

#include <signal.h>
#include "common/dispatch.hpp"
#include "common/resources/unique_fd.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* controller */
	class controller;


	// -- S I G N A L ---------------------------------------------------------

	class signal final : public sm::listener<sm::controller> {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::signal;


			// -- private members ---------------------------------------------

			/* pipe */
			sm::unique_fd _pipe[2U];


			// -- private static methods --------------------------------------


			/* signal handler */
			static void _handler(const int) noexcept;

			/* record */
			static auto _record(const int&) -> void;

			/* record */
			template <int... sigs>
			static auto _record(void) -> void {
				(self::_record(sigs), ...);
			}

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			signal(void);

			/* deleted copy constructor */
			signal(const self&) = delete;

			/* deleted move constructor */
			signal(self&&) = delete;

			/* destructor */
			~signal(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


		public:

			// -- public static methods ---------------------------------------

			/* shared */
			static auto shared(void) -> self&;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(sm::controller&, const sm::event&) -> void override;

	}; // class signal

} // namespace sm

#endif // signal_hpp
