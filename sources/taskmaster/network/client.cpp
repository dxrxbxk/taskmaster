#include "taskmaster/network/client.hpp"
#include "taskmaster/log/logger.hpp"
#include <iostream>
#include "taskmaster/taskmaster.hpp"
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
auto sm::client::on_event(const sm::event& event) -> void {


	if (event.is_hup()
	 || event.is_rdhup()) {
		self::_disconnect();
		return;
	}

	if (not event.is_in())
		return;

	sm::reader<128U> reader;

	reader.receive(_socket);

	if (reader.size() == 0U) {
		self::_disconnect();
		return;
	}

	std::string str{"client: received message: "};
	str.append(reader.data(), reader.size());

	sm::logger::info(str.data());
}


// -- private methods ---------------------------------------------------------

/* disconnect */
auto sm::client::_disconnect(void) -> void {
	sm::taskmaster::monitor().unsubscribe(*this);
	sm::taskmaster::clients().remove(*this);
	sm::logger::info("client: disconnected");
}
