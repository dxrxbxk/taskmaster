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




	template <typename T>
	class timer2 : public sm::listener {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::timer2<T>;

			/* method type */
			using method_type = auto (T::*)(sm::taskmaster&) -> void;


			// -- private members ---------------------------------------------

			/* file descriptor */
			sm::unique_fd _fd;

			/* object */
			T* _object;

			/* method */
			method_type _method;


			// -- private static methods --------------------------------------

			/* create */
			static auto _create(const int& clock_id = CLOCK_REALTIME,
								const int& flags = TFD_NONBLOCK) -> int {

				const auto fd = ::timerfd_create(clock_id, flags);

				if (fd == -1)
					throw sm::system_error("timerfd_create");

				return fd;
			}


			// -- private methods ---------------------------------------------

			/* set relative */
			auto _set_relative(const struct ::timespec& value,
							   const struct ::timespec& interval) const -> void {

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

			/* consume */
			auto _consume(void) const -> ::uint64_t {

				::uint64_t expirations;

				// read timer
				if (::read(_fd, &expirations, sizeof(expirations)) == -1)
					throw sm::system_error(__func__);

				return expirations;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			timer2(void) noexcept = default;

			/* time constructor */
			timer2(T& obj, method_type method, const sm::isize& ms)
			: sm::timer2<T>{obj, method, ms, 0} {
			}

			/* interval constructor */
			timer2(T& obj, method_type method, const sm::isize& ms_value, const sm::isize& ms_interval)
			: sm::listener{}, _fd{self::_create()}, _object{&obj}, _method{method} {

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

			/* deleted copy constructor */
			timer2(const self&) = delete;

			/* move constructor */
			timer2(self&&) noexcept = default;

			/* destructor */
			~timer2(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return _fd;
			}

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster& tm) -> void override {

				// consume timer
				const auto _ = this->_consume();

				// call method
				(_object->*_method)(tm);
			}

	}; // class timer

} // namespace sm

#endif // timer_hpp
