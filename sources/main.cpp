#include "taskmaster.hpp"
#include "options.hpp"
#include "log/logger.hpp"
#include "environment.hpp"
// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌

#include <csignal>

auto main(int ac, char** av, char **env) -> int {

	// ignore signals
	//::signal(SIGTERM, SIG_IGN);
	//
	//while (true) {
	//	std::cout << "Hello, World!" << std::endl;
	//	sleep(1);
	//}
	//
	//return 0;
	//
	
	try {
	//for (int i = 0; env[i] != nullptr; ++i) {
	//	std::cout << env[i] << std::endl;
	//}


	auto env2 = sm::environment::copy();

	env2.for_each([](const char* var) {
		std::cout << var << std::endl;
	});




	// check if standard input / output is a terminal
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO)) {
		sm::logger::error("Standard input/output is not a terminal");
		return EXIT_FAILURE;
	}



		// parse command line options
		sm::options opts{ac, av};

		// launch program
		sm::taskmaster::run(opts);

		return EXIT_SUCCESS;
	}

	// main exception handler
	catch (const sm::exception& e) {

		// log error
		sm::logger::error(std::string_view{e.what()});

		return EXIT_FAILURE;
	}

	// child process exception handler
	catch (const sm::exit& e) {

		// return status
		return e.status();
	}
}
