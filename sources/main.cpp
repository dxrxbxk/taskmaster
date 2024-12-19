#include "taskmaster.hpp"
#include "options.hpp"
#include "log/logger.hpp"

// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌


#include "config/parser.hpp"
#include "stream.hpp"

#include "config/parser2.hpp"
#include <pwd.h>

#include "diagnostics/error.hpp"

#include <math.h>


#include <readline/readline.h>

#include "terminal/terminal.hpp"

#include "box.hpp"
#include "utils/limits.hpp"


auto main(int ac, char** av) -> int {

	//signal(SIGTERM, SIG_IGN);
	//
	//while (true) {
	//	std::cout << "-" << std::endl;
	//	sleep(1);
	//}
	//
	//return 0;

	//sm::termios_test<true>();
	//return 0;

	try {

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
