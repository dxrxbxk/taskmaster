#include "taskmaster/taskmaster.hpp"
#include "taskmaster/options.hpp"
#include "taskmaster/log/logger.hpp"

// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌


#include "taskmaster/config/parser.hpp"


auto main(int ac, char** av) -> int {

	//std::cout << "Hello, world!" << std::endl;
	//std::cout << "This is the Taskmaster project." << std::endl;
	//
	//sleep(5);
	//
	//return 0;


	try {

		//sm::parser_tester();
		//
		//return 0;

		// parse command line options
		sm::options opts{ac, av};

		// launch program
		sm::taskmaster::run(opts);

		return EXIT_SUCCESS;
	}

	catch (const sm::exception& e) {

		// log error
		sm::logger::error(e.what());

		return EXIT_FAILURE;
	}

	catch (const int&) {
		return EXIT_SUCCESS;
	}
}
