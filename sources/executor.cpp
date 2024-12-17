#include "taskmaster.hpp"
#include "executor.hpp"
#include "log/logger.hpp"


// -- private methods ---------------------------------------------------------

/* stop */
auto sm::executor::_stop(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 2U) {
		sm::logger::warn("usage: stop <program>");
		return;
	}

	// not implemented yet...
	sm::logger::info("stop command");
}

/* list */
auto sm::executor::_list(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 1U) {
		sm::logger::warn("usage: list");
		return;
	}

	tm.programs().info();
}

/* start */
auto sm::executor::_start(sm::taskmaster& tm, const argv_type& argv) -> void {

	sm::logger::info("start command");
}

/* help */
auto sm::executor::_help(sm::taskmaster& tm, const argv_type& argv) -> void {

	sm::logger::info("help command");
}

/* exit */
auto sm::executor::_exit(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 1U) {
		sm::logger::warn("usage: exit");
		return;
	}

	// stop the taskmaster
	tm.stop();
}


// -- public methods ----------------------------------------------------------

/* execute */
auto sm::executor::execute(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.empty()) {
		sm::logger::warn("empty command");
		return;
	}

	auto it = _functions.find(argv[0]);

	// if the command is found
	if (it == _functions.end()) {
		sm::logger::warn("command not found");
		return;
	}

	// execute the command
	(this->*(it->second))(tm, argv);
}
