#include "inotify.hpp"
#include "taskmaster.hpp"


/* events */
auto sm::inotify::on_event(const sm::event& ev, sm::taskmaster& tm) -> void {

	// buffer
	char buffer[4096U];


	// read events
	const auto bytes = ::read(_fd, buffer, 4096U);

	// check error
	if (bytes == -1)
		throw sm::system_error("read");

	// loop over events
	for (unsigned i = 0U; i < bytes; ) {

		// event
		const auto* event = reinterpret_cast<struct ::inotify_event*>(buffer + i);

		// get notifiable
		const auto it = _map.find(event->wd);

		// check notifiable
		if (it == _map.end())
			continue;

		// or event
		_events[it->second] |= event->mask;

		// next event
		i += sizeof(struct ::inotify_event) + event->len;
	}


	if (bytes == 0)
		return;

	// set new timer
	_timer = sm::timer2<self>{*this, &self::handle_events, 10U};

	tm.monitor().subscribe(_timer, sm::event{EPOLLIN});
}


auto sm::inotify::handle_events(sm::taskmaster& tm) -> void {

	// loop over events
	for (const auto& [notifiable, mask] : _events) {
		notifiable->on_event(mask, tm);
	}

	// clear events
	_events.clear();

	// unsubscribe timer
	tm.monitor().unsubscribe(_timer);
}
