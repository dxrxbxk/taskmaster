#include "taskmaster/taskmaster.hpp"
#include "taskmaster/options.hpp"
#include "taskmaster/log/logger.hpp"

// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌


#include "taskmaster/config/parser.hpp"
#include "common/stream.hpp"
#include "common/atoi.hpp"


#include <pwd.h>

auto main(int ac, char** av) -> int {

	//std::cout << "Hello, world!" << std::endl;
	//std::cout << "This is the Taskmaster project." << std::endl;
	//
	//sleep(5);
	//
	//return 0;



	try {


		//auto uid = ::getuid();
		//
		//struct ::passwd* pw = ::getpwuid(uid);
		//
		//if (pw == nullptr) {
		//	throw sm::system_error{"getpwuid"};
		//}
		//
		//sm::logger::info(pw->pw_name);
		//sm::logger::info(pw->pw_dir);
		//sm::logger::info(pw->pw_shell);
		//sm::logger::info(pw->pw_passwd);
		//
		//
		//
		//return 0;

		//char buffer[1024];
		//
		//sm::stream s{buffer, sizeof(buffer)};
		//
		//s.print(true, ' ', false, ' ', 42, '=', " goodbye, world!\n");
		//
		//std::cout << buffer << std::flush;
		//
		//
		//return 0;

		//sm::parser_tester();
		//
		//return 0;

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
