#include "config/config.hpp"
#include "config/parser.hpp"
#include "resources/unique_fd.hpp"
#include "reader.hpp"
#include "system/open.hpp"
#include "taskmaster.hpp"

#include <sys/stat.h>
#include <pwd.h>


// -- C O N F I G -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::config::config(void)
: _path{} {
}

/* path constructor */
sm::config::config(const std::string& path)
: _path{path} {
}

/* move path constructor */
sm::config::config(std::string&& path) noexcept
: _path{std::move(path)} {
}


// -- public methods ----------------------------------------------------------

/* parse */
auto sm::config::parse(sm::program_manager& pm) -> void {

	if (_path.empty()) {

		// get user name
		const auto* pw = ::getpwuid(::getuid());

		if (pw == nullptr)
			throw sm::system_error{"getpwuid"};

		_path.assign(pw->pw_dir);
		_path.append("/.config/taskmaster/taskmaster.conf");
	}

	sm::unique_fd file = sm::open(_path.data(), O_RDONLY);

	self::_parse(file, pm);
}


// -- public modifiers --------------------------------------------------------

/* path */
auto sm::config::path(const std::string& path) -> void {
	_path.assign(path);
}

/* move path */
auto sm::config::path(std::string&& path) noexcept -> void {
	_path.assign(std::move(path));
}

/* reload */
auto sm::config::reload(sm::taskmaster& tm) -> void {

	sm::logger::hint("config file: reloading...");
	sm::program_manager pm;

	try {
		sm::unique_fd file = sm::open(_path.data(), O_RDONLY);

		self::_parse(file, pm);
	}
	catch (const sm::exception& e) {
		sm::logger::error("error while reloading config file: ", std::string_view{e.what()});
		return;
	}

	tm.programs().hot_swap(tm.monitor(), std::move(pm));
	tm.readline().prompt();
}


// -- public overrides --------------------------------------------------------

/* path */
auto sm::config::path(void) const noexcept -> const char* {
	return _path.data();
}

/* on event */
auto sm::config::on_event(const ::uint32_t& ev, sm::taskmaster& tm) -> void {


	if (ev & (IN_DELETE_SELF | IN_MOVE_SELF)) {

		// check if file path is still valid
		if (::access(_path.data(), F_OK) == -1) {
			sm::logger::warn("config file deleted");
			tm.readline().prompt();
			return;
		}
	}
	else if (ev & IN_MODIFY) {
		; // do nothing
	}
	else {
		throw sm::runtime_error{"unexpected event"};
	}

	tm.reload();

	tm.inotify().watch(*this, IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF | IN_ONESHOT);
}


// -- private static methods --------------------------------------------------

/* parse */
auto sm::config::_parse(const sm::unique_fd& file, sm::program_manager& pm) -> void {

	sm::reader<1024U> reader;
	sm::parser parser;

	do {

		reader.read(file);
		parser.parse(reader, pm);

	} while (reader.eof() == false);
}
