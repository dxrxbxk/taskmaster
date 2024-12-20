#ifndef timer_hpp
#define timer_hpp

#include <sys/timerfd.h>
#include <unistd.h>
#include "diagnostics/exception.hpp"
#include "resources/unique_fd.hpp"
#include "events/monitor.hpp"
#include "log/logger.hpp"



// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* process */
	class process;


	// -- T I M E R -----------------------------------------------------------

	class timer : public sm::listener {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::timer;

			/* method type */
			using method_type = auto (sm::process::*)(sm::taskmaster&) -> void;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* program */
			sm::process* _program;

			/* method */
			method_type _method;


			// -- private static methods --------------------------------------

			/* create */
			static auto _create(const int& = CLOCK_REALTIME,
								const int& = TFD_NONBLOCK) -> int;


			// -- private methods ---------------------------------------------

			/* set relative */
			auto _set_relative(const struct ::timespec&,
							   const struct ::timespec&) const -> void;

			/* consume */
			auto _consume(void) const -> ::uint64_t;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			timer(void) noexcept = default;

			/* time constructor */
			timer(sm::process&, method_type, const sm::isize&);

			/* interval constructor */
			timer(sm::process&, method_type,
					const sm::isize&, const sm::isize&);

			/* deleted copy constructor */
			timer(const self&) = delete;

			/* move constructor */
			timer(self&&) noexcept = default;

			/* destructor */
			~timer(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;

	}; // class timer

} // namespace sm

#endif // timer_hpp
