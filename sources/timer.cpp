#include "time/timer.hpp"
#include "program.hpp"


// -- T I M E R ---------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* time constructor */
sm::timer::timer(sm::program& prg, method_type method,
								const sm::isize& ms)
: sm::timer{prg, method, ms, 0} {
}

/* interval constructor */
sm::timer::timer(sm::program& prg, method_type method,
		const sm::isize& ms_value, const sm::isize& ms_interval)
: sm::listener{}, _fd{self::_create()}, _program{&prg}, _method{method} {


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


// -- private static methods --------------------------------------------------

/* create */
auto sm::timer::_create(const int& clock_id, const int& flags) -> int {

	const auto fd = ::timerfd_create(clock_id, flags);

	if (fd == -1)
		throw sm::system_error("timerfd_create");

	return fd;
}

/* set relative */
auto sm::timer::_set_relative(const struct ::timespec& value,
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


// -- private methods ---------------------------------------------------------

/* consume */
auto sm::timer::_consume(void) const -> ::uint64_t {

	::uint64_t expirations;

	// read timer
	if (::read(_fd, &expirations, sizeof(expirations)) == -1)
		throw sm::system_error(__func__);

	return expirations;
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::timer::fd(void) const noexcept -> int {
	return _fd;
}

/* on event */
auto sm::timer::on_event(const sm::event& events, sm::taskmaster& tm) -> void {

	// consume timer
	const auto _ = this->_consume();

	// call method
	(_program->*_method)(tm);
}
