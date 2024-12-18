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

	//sm::termios_test<true>();
	//return 0;

	try {

		//std::cout << -123 << std::endl;
		//std::cout << -123 % 10 << std::endl;
		//std::cout << -123 / 10 << std::endl;
		//
		//return 0;

		char buffer[1024];
		sm::stream ss{buffer, sizeof(buffer)};

		//ss.append("hello world", ' ', 42, "is", true, '\n');
		ss.append((int)-2);

		std::cout << buffer << std::flush;

		return 0;

		std::cout << sm::limits<unsigned short>::max() << std::endl;
		std::cout << sm::limits<unsigned short>::min() << std::endl;

		std::cout << sm::limits<signed short>::max() << std::endl;
		std::cout << sm::limits<signed short>::min() << std::endl;

		std::cout << sm::limits<unsigned int>::max() << std::endl;
		std::cout << sm::limits<unsigned int>::min() << std::endl;

		std::cout << sm::limits<signed int>::max() << std::endl;
		std::cout << sm::limits<signed int>::min() << std::endl;

		return 0;


		// parse command line options
		sm::options opts{ac, av};

		// launch program
		sm::taskmaster::run(opts);

		return EXIT_SUCCESS;
	}

	// main exception handler
	catch (const sm::exception& e) {

		// log error
		sm::logger::error(e.what());

		return EXIT_FAILURE;
	}

	// child process exception handler
	catch (const sm::exit& e) {

		// return status
		return e.status();
	}
}
