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
#include "type_traits/type_traits.hpp"

#include <type_traits>

class A {
	public:
		A() {
			std::cout << "A()" << std::endl;
		}
		~A() = default;
		A(const A&) {
			std::cout << "A(const A&)" << std::endl;
		}

		A(A&&) {
			std::cout << "A(A&&)" << std::endl;
		}
};

template <typename T>
void test_forward(T&& t) {
	A obj = sm::forward<T>(t);
}


auto main(int ac, char** av) -> int {

	//sm::make_unsigned_t<int> b = 42;
	//
	//std::cout << std::boolalpha << sm::is_same<decltype(b), unsigned int> << std::endl;
	//std::make_unsigned_t<int> a = 42;
	//
	//A obj;
	//
	//std::cout << "testing lvalue: ";
	//test_forward(obj);
	//std::cout << "testing rvalue: ";
	//test_forward(A{});
	//
	//// move test
	//std::cout << "testing move: ";
	//A obj2 = sm::move(obj);
	//
	//return 0;
	//
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
