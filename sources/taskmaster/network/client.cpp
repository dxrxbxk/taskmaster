#include "taskmaster/network/client.hpp"
#include "taskmaster/log/logger.hpp"
#include <iostream>
#include "taskmaster/taskmaster.hpp"


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
auto sm::client::on_event(const sm::event& event, sm::taskmaster& tm) -> void {


	if (event.is_hup()
	 || event.is_rdhup()
	 || event.is_err()) {
		self::_disconnect(tm);
		return;
	}

	if (not event.is_in())
		return;

	sm::reader<128U> reader;

	reader.receive(_socket);

	if (reader.size() == 0U) {
		self::_disconnect(tm);
		return;
	}

	std::string str{"client: received message: "};
	str.append(self::_to_hex(reader));

	sm::logger::info(str.data());
}


// -- private methods ---------------------------------------------------------

/* disconnect */
auto sm::client::_disconnect(sm::taskmaster& tm) -> void {
	tm.monitor().unsubscribe(*this);
	tm.clients().remove(*this);
	sm::logger::info("client: disconnected");
}
