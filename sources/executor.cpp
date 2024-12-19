#include "taskmaster.hpp"
#include "executor.hpp"
#include "log/logger.hpp"

#include <sys/wait.h>


// -- private methods ---------------------------------------------------------

/* stop */
auto sm::executor::_stop(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 2U) {
		sm::logger::warn("usage: stop <program>");
		return;
	}

	// check if program exists
	if (not tm.programs().has_program(argv[1])) {
		sm::logger::warn("program not found");
		return;
	}

	// get the program
	auto& prog = tm.programs().get_program(argv[1U]);

	// get processes
	auto& procs = prog.processes();

	// stop all processes
	for (auto& proc : procs)
		proc.stop(tm);
}

/* list */
auto sm::executor::_info(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 1U) {
		sm::logger::warn("usage: list");
		return;
	}

	tm.programs().info();
}

/* status */
auto sm::executor::_status(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 2U) {
		sm::logger::warn("usage: status <program>");
		return;
	}

	// check if program exists
	if (not tm.programs().has_program(argv[1U])) {
		sm::logger::warn("program not found");
		return;
	}

	// get the program
	auto& prog = tm.programs().get_program(argv[1U]);

	// get processes
	auto& procs = prog.processes();

	// get the status of all processes
	for (auto& proc : procs)
		proc.status();
}

/* start */
auto sm::executor::_start(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 2U) {
		sm::logger::warn("usage: start <program>");
		return;
	}

	// check if program exists
	if (not tm.programs().has_program(argv[1])) {
		sm::logger::warn("program not found");
		return;
	}

	// get the program
	auto& prog = tm.programs().get_program(argv[1U]);

	// get processes
	auto& procs = prog.processes();

	// start all processes
	for (auto& proc : procs)
		proc.start(tm);
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

/* fg */
auto sm::executor::_fg(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 2U) {
		sm::logger::warn("usage: fg <program>");
		return;
	}
	sm::logger::warn("not implemented yet");

	/*
	if (not tm.programs().has_program(argv[1])) {
		sm::logger::warn("program not found");
		return;
	}

	auto& program = tm.programs().get_program(argv[1]);


	auto pid = program.pid();

	if (pid == 0) {
		sm::logger::warn("program not running");
		return;
	}

	// get back stdout and stderr
	int fd = open("/dev/tty", O_RDWR);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);

	// set the program to the foreground


	tcsetpgrp(STDIN_FILENO, pid);

	// wait for the program to finish
	int status;
	waitpid(pid, &status, WUNTRACED);

	tcsetpgrp(STDIN_FILENO, getpgrp());

	// set the program back to the taskmaster


	// not implemented yet...
	sm::logger::info("fg command");
	*/
}

/* clear */
auto sm::executor::_clear(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.size() != 1U) {
		sm::logger::warn("usage: clear");
		return;
	}

	// clear the screen
	::write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7U);
}

/* not implemented */
auto sm::executor::_not_implemented(sm::taskmaster&, const argv_type&) -> void {
	sm::logger::warn("not implemented yet");
}


// -- public methods ----------------------------------------------------------

/* execute */
auto sm::executor::execute(sm::taskmaster& tm, const argv_type& argv) -> void {

	if (argv.empty()) {
		sm::logger::warn("empty command");
		return;
	}

	auto it = _functions.find(argv[0U]);

	// if the command is found
	if (it == _functions.end()) {
		sm::logger::warn("command not found: ", argv[0U]);
		return;
	}

	// execute the command
	(this->*(it->second))(tm, argv);
}
