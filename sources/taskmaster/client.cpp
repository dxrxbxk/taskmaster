#include "taskmaster/client.hpp"
#include <iostream>
#include "taskmaster/controller.hpp"
#include "common/reader.hpp"


// -- C L I E N T -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* socket constructor */
sm::client::client(sm::socket&& socket) noexcept
: _socket{static_cast<sm::socket&&>(socket)} {
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::client::fd(void) const noexcept -> int {
	return _socket;
}

/* on event */
auto sm::client::on_event(sm::controller& controller,
							const sm::event& event) -> void {
	static int i = 0;

	//std::cout << i++ << std::endl;

	if (event.is_hup() or event.is_rdhup()) {

		controller.monitor().unsubscribe(*this);
		controller.clients().remove(*this);
		std::cout << "client shutdown" << std::endl;
		return;
	}

	if (not event.is_in())
		return;

	sm::reader<128U> reader;

	reader.receive(_socket);

	if (reader.size() == 0U) {
		controller.monitor().unsubscribe(*this);
		controller.clients().remove(*this);
		std::cout << "client disconnected" << std::endl;
		return;
	}

	::write(STDOUT_FILENO, reader.data(), reader.size());
}
