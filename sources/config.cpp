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
: _path{"/home/richtofen/Code/taskmaster/tools/taskmaster.conf"} {
//: _path{"/root/data/taskmaster/tools/taskmaster.conf"} {


	/*
	// get uid
	const auto uid = ::getuid();

	// get home
	const auto* pw = ::getpwuid(uid);
	if (pw == nullptr)
		throw sm::system_error{"getpwuid"};

	std::string path{pw->pw_dir};
	path.append("/.config/taskmaster/taskmaster.conf");

	struct ::stat st;

	const auto ret = ::stat(path.data(), &st);

	if (ret == 0 && S_ISREG(st.st_mode) && (st.st_mode & S_IRUSR)) {
		_path.assign(std::move(path));
		return;
	}

	if (errno != ENOENT)
		throw sm::system_error{"stat"};


	sm::logger::info("config file: ", path);
	*/
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

	struct ::stat st;

	const auto ret = ::stat(_path.data(), &st);

	if (ret == -1) {

		if (errno == ENOENT)
			throw sm::runtime_error{"config file not found"};

		throw sm::system_error{"stat"};
	}

	// check if file is regular
	if (S_ISREG(st.st_mode) == false)
		throw sm::runtime_error{"config file is not regular file"};

	// check if file is readable
	if ((st.st_mode & S_IRUSR) == 0)
		throw sm::runtime_error{"config file is not readable"};


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


// -- public overrides --------------------------------------------------------

/* path */
auto sm::config::path(void) const noexcept -> const char* {
	return _path.data();
}

/* on event */
auto sm::config::on_event(const ::uint32_t& ev, sm::taskmaster& tm) -> void {


	if (ev & IN_DELETE_SELF) {

		// check if file path is still valid
		if (::access(_path.data(), F_OK) == -1) {
			sm::logger::warn("config file deleted");
			tm.readline().prompt();
			return;
		}

		tm.inotify().watch(*this, IN_MODIFY | IN_DELETE_SELF | IN_MOVE_SELF);
	}

	else if (ev & IN_MOVE_SELF) {
		sm::logger::warn("config file moved");
		tm.readline().prompt();
		return;
	}
	else if (ev & IN_MODIFY) {
		; // do nothing
	}
	else {
		throw sm::runtime_error{"unexpected event"};
	}


	sm::logger::warn("re-parsing config file");
	sm::program_manager pm;
	sm::unique_fd file = sm::open(_path.data(), O_RDONLY);

	self::_parse(file, pm);


	tm.programs() = std::move(pm);
	sm::logger::info("config file reloaded");
	tm.readline().prompt();
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
