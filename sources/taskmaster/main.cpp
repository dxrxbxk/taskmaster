#include "taskmaster/taskmaster.hpp"
#include "taskmaster/options.hpp"
#include "taskmaster/log/logger.hpp"

// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌


#include "common/reader.hpp"
#include "taskmaster/config/parser.hpp"

auto main(int ac, char** av) -> int {


	try {

		sm::parser_tester();

		return 0;

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
}

	//int i = 0;
	//
	//for (;i < 256; ++i) {
	//
	//	if (i <= 32 || i >= 127) {
	//
	//		// horizontal tab, vertical tab, form feed, backspace, space
	//		if (i == '\t' || i == ' ') {
	//			std::cout << "C_WHITESPACE,\n";
	//		}
	//		// newline
	//		else if (i == '\n' || i == '\v' || i == '\f') {
	//			std::cout << "C_NEWLINE,\n";
	//		}
	//		// return
	//		else if (i == '\r') {
	//			std::cout << "C_RETURN,\n";
	//		}
	//		else {
	//			std::cout << "C_INVALID,\n";
	//		}
	//	}
	//	else {
	//
	//		if (i == '=')
	//			std::cout << "C_EQUAL,\n";
	//		else if (i == '\'')
	//			std::cout << "C_SIMPLE,\n";
	//		else if (i == '\"')
	//			std::cout << "C_DOUBLE,\n";
	//		else if (i == ',')
	//			std::cout << "C_COMMA,\n";
	//		else if (i == '$')
	//			std::cout << "C_DOLLAR,\n";
	//		else if (i == '#')
	//			std::cout << "C_HASH,\n";
	//		else if (i == '[')
	//			std::cout << "C_LEFT_BRACKET,\n";
	//		else if (i == ']')
	//			std::cout << "C_RIGHT_BRACKET,\n";
	//		else if ((i >= '0' && i <= '9'))
	//			std::cout << "C_DIGIT,\n";
	//		else if ((i >= 'a' && i <= 'z') || (i >= 'A' && i <= 'Z'))
	//			std::cout << "C_ALPHA,\n";
	//		else if (i == '_')
	//			std::cout << "C_UNDERSCORE,\n";
	//		else
	//			std::cout << "C_SYMBOL,\n";
	//
	//	}
	//}
	//return 0;
