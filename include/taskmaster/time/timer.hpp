#ifndef timer_hpp
#define timer_hpp

#include <sys/timerfd.h>
#include <unistd.h>
#include "common/diagnostics/exception.hpp"
#include "common/resources/unique_fd.hpp"
#include "taskmaster/events/monitor.hpp"
#include "taskmaster/log/logger.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class timer : public sm::listener {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::timer;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;


			// -- private static methods --------------------------------------

			/* create */
			template <int clock_id = CLOCK_REALTIME, int flags = TFD_NONBLOCK>
			static auto _create(void) -> int {

				const auto fd = ::timerfd_create(clock_id, flags);

				if (fd == -1)
					throw sm::system_error("timerfd_create");

				return fd;
			}


			/* set relative */
			auto _set_relative(const struct ::timespec& value,
							   const struct ::timespec& interval) -> void {

				const struct ::itimerspec its {
					interval,
					value
				};

				// args:
				// 1. file descriptor
				// 2. flags (0 for relative, TFD_TIMER_ABSTIME for absolute)
				// 3. new timer value
				// 4. old timer value

				if (::timerfd_settime(_fd, 0, &its, nullptr) == -1)
					throw sm::system_error("timerfd_settime");
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			timer(void) noexcept = default;

			/* time constructor */
			timer(const sm::isize& ms)
			: sm::timer{ms, 0} {
			}

			/* interval constructor */
			timer(const sm::isize& ms_value, const sm::isize& ms_interval)
			: sm::listener{}, _fd{self::_create<CLOCK_REALTIME, TFD_NONBLOCK>()} {

				const struct ::timespec ts_value {
					.tv_sec  =  ms_value / 1000,
					.tv_nsec = (ms_value % 1000) * 1000000
				};

				const struct ::timespec ts_interval {
					.tv_sec  =  ms_interval / 1000,
					.tv_nsec = (ms_interval % 1000) * 1000000
				};

				// set timer
				self::_set_relative(ts_value, ts_interval);
			}


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return _fd;
			}

			/* on event */
			auto on_event(const sm::event& events, sm::taskmaster& tm) -> void override {

				// consume timer
				const auto expirations = this->consume();

				sm::logger::info("timer triggered");
			}


			// -- public methods ----------------------------------------------

			/* consume */
			auto consume(void) -> ::uint64_t {

				::uint64_t expirations;

				// read timer
				if (::read(_fd, &expirations, sizeof(expirations)) == -1)
					throw sm::system_error(__func__);

				return expirations;
			}

	}; // class timer


} // namespace sm

#endif // timer_hpp
