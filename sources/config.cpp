#include "config/config.hpp"
#include "config/parser.hpp"
#include "resources/unique_fd.hpp"
#include "reader.hpp"
#include "system/open.hpp"

#include <sys/stat.h>


// -- C O N F I G -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::config::config(void)
: _path{"/home/richtofen/Code/taskmaster/tools/taskmaster.conf"} {
//: _path{"/root/data/taskmaster/tools/taskmaster.conf"} {

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

	sm::reader<1024U> reader;
	sm::parser parser;

	do {

		reader.read(file);
		parser.parse(reader, pm);

	} while (reader.eof() == false);
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
