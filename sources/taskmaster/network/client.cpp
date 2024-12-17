#include "taskmaster/network/client.hpp"
#include "taskmaster/log/logger.hpp"
#include <iostream>
#include "taskmaster/taskmaster.hpp"
#include "common/string/split.hpp"
#include "taskmaster/builtins/command_factory.hpp"


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


	try {

		if (event.is_err()) {
			sm::logger::error("client: error event");
			self::_disconnect(tm);
			return;
		}

		if (event.is_in()) {

			char buffer[1024U];

			const auto bytes = ::recv(_socket, buffer, sizeof(buffer), 0);

			switch (bytes) {
				case -1:
					sm::logger::error(sm::system_error{"client recv: "}.what());
					/* fallthrough */
				case 0:
					self::_disconnect(tm);
					return;
				default: break;
			}

			// log received bytes
			std::string msg{"client: received message: "};
			msg.append(self::_to_hex(buffer, static_cast<unsigned>(bytes)));
			sm::logger::info(msg.data());
			msg.clear();


			// append to buffer
			_buffer.append(buffer, static_cast<unsigned>(bytes));

			// find '\r\n'
			const auto pos = _buffer.find("\r\n");

			if (pos != std::string::npos) {

				// extract line
				const auto line = _buffer.substr(0U, pos);
				// remove line from buffer
				_buffer.erase(0U, pos + 2U);


				msg.assign("client: received command: ");
				msg.append(line);

				sm::logger::info(msg.data());


				// split line by space
				auto argv = sm::split(line, " \t\v\f");

				// create command
				auto cmd = sm::command_factory::create(std::move(argv));

				// execute command
				cmd->execute(tm);
			}
		}


		// handle disconnect
		if (event.is_hup() || event.is_rdhup())
			self::_disconnect(tm);
	}

	catch (const sm::exception& e) {
		sm::logger::error(e.what());
		self::_disconnect(tm);
	}
}


// -- private methods ---------------------------------------------------------

/* disconnect */
auto sm::client::_disconnect(sm::taskmaster& tm) -> void {
	tm.monitor().unsubscribe(*this);
	tm.clients().remove(*this);
	sm::logger::info("client: disconnected");
}
