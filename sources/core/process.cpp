#include "core/process.hpp"
#include "core/profile.hpp"
#include "log/logger.hpp"
#include <string_view>


// -- P R O C E S S -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* profile constructor */
sm::process::process(const sm::shared_ptr<sm::profile>& profile)
: _profile{profile},
  _state{S_STOPPED},
  _pid{0},
  _pidfd{} {
}

/* destructor */
sm::process::~process(void) noexcept {
}


// -- public overrides --------------------------------------------------------

/* fd */
auto sm::process::fd(void) const noexcept -> int {
	return _pidfd;
}

/* on_event */
auto sm::process::on_event(const sm::event&, sm::taskmaster&) -> void {


	if (_profile->autorestart() == true)
		return;
}


// -- public methods ----------------------------------------------------------

/* status */
auto sm::process::status(void) const -> void {

	// get command
	const std::string_view cmd{_profile->cmd()[0U]};

	switch (_state) {

		case S_STOPPED:
			sm::logger::info("process: ", cmd, " [STOPPED]");
			break;

		case S_STARTING:
			sm::logger::info("process: ", cmd, " [STARTING]");
			break;

		case S_RUNNING:
			sm::logger::info("process: ", cmd, " [RUNNING]");
			break;

		case S_BACKOFF:
			sm::logger::info("process: ", cmd, " [BACKOFF]");
			break;

		case S_STOPPING:
			sm::logger::info("process: ", cmd, " [STOPPING]");
			break;

		case S_EXITED:
			sm::logger::info("process: ", cmd, " [EXITED]");

		case S_FATAL:
			sm::logger::info("process: ", cmd, " [FATAL]");
			break;

		default:
			sm::logger::info("process: ", cmd, " [UNKNOWN]");
			break;
	}
}


// -- public accessors --------------------------------------------------------

/* is running */
auto sm::process::is_running(void) const noexcept -> bool {
	return _pid != 0;
}

/* pid */
auto sm::process::pid(void) const noexcept -> const ::pid_t& {
	return _pid;
}






