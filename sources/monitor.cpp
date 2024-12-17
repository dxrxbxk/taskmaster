#include "events/monitor.hpp"
#include "taskmaster.hpp"


// -- M O N I T O R -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::monitor::monitor(void)
: _fd{::epoll_create1(0)} {

	if (_fd == -1)
		throw sm::system_error("epoll_create1");
}


// -- public methods ----------------------------------------------------------

/* subscribe */
auto sm::monitor::subscribe(sm::listener& listener, const sm::event& events) -> void {

	// create event
	struct ::epoll_event ev {
		.events = events,
			.data = {
				.ptr = &listener
			}
	};

	// add event
	if (::epoll_ctl(_fd, EPOLL_CTL_ADD, listener.fd(), &ev) == -1)
		throw sm::system_error("epoll_ctl");

	// add event to list
	_events.resize(_events.size() + 1U);
}

/* unsubscribe */
auto sm::monitor::unsubscribe(sm::listener& listener) noexcept -> void {

	// delete event
	static_cast<void>(::epoll_ctl(_fd, EPOLL_CTL_DEL, listener.fd(), nullptr));
}

/* wait */
auto sm::monitor::wait(sm::taskmaster& tm) -> void {

	// wait for events
	const auto state = ::epoll_wait(_fd,
			_events.data(),
			static_cast<int>(_events.size()),
			-1);

	// check for error
	if (state == -1) {

		// check for interruption
		if (errno == EINTR || errno == EAGAIN)
			return;

		throw sm::system_error("epoll_wait");
	}

	// cast to size
	const auto nevents = static_cast<sm::usize>(state);

	// loop over events
	for (sm::u32 i = 0U; i < nevents && tm.is_running(); ++i) {

		// get event
		const auto ev = sm::event{_events[i].events};

		// get listener
		auto& listener = *(reinterpret_cast<sm::listener*>(_events[i].data.ptr));

		// trigger event
		listener.on_event(ev, tm);
	}

}
