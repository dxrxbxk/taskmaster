#include "taskmaster/taskmaster.hpp"
#include "taskmaster/options.hpp"
#include "taskmaster/log/logger.hpp"

// ▗▄▄▄▖▗▄▖  ▗▄▄▖▗▖ ▗▖▗▖  ▗▖ ▗▄▖  ▗▄▄▖▗▄▄▄▖▗▄▄▄▖▗▄▄▖
//   █ ▐▌ ▐▌▐▌   ▐▌▗▞▘▐▛▚▞▜▌▐▌ ▐▌▐▌     █  ▐▌   ▐▌ ▐▌
//   █ ▐▛▀▜▌ ▝▀▚▖▐▛▚▖ ▐▌  ▐▌▐▛▀▜▌ ▝▀▚▖  █  ▐▛▀▀▘▐▛▀▚▖
//   █ ▐▌ ▐▌▗▄▄▞▘▐▌ ▐▌▐▌  ▐▌▐▌ ▐▌▗▄▄▞▘  █  ▐▙▄▄▖▐▌ ▐▌


#include "taskmaster/config/parser.hpp"
#include "common/stream.hpp"


#include "taskmaster/config/parser2.hpp"
#include <pwd.h>

#include <math.h>
auto main(int ac, char** av) -> int {


	try {


		//sm::parser_tester2();
		//
		//return 0;


		//auto num = sm::atoi<unsigned char, sm::oct>("57");
		//
		//std::cout << "num: " << (int)num
		//		  << "\nhex: " << std::hex << (int)num
		//		  << "\noct: " << std::oct << (int)num
		//		  << std::endl;
		//
		//
		//
		//return 0;
		//
		//std::string ss[] {
		//	"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP",
		//	"SIGABRT", "SIGIOT", "SIGBUS", "SIGFPE", "SIGKILL",
		//	"SIGUSR1", "SIGSEGV", "SIGUSR2", "SIGPIPE", "SIGALRM",
		//	"SIGTERM", "SIGSTKFLT", "SIGCHLD", "SIGCONT",
		//	"SIGSTOP", "SIGTSTP", "SIGTTIN", "SIGTTOU", "SIGURG",
		//	"SIGXCPU", "SIGXFSZ", "SIGVTALRM", "SIGPROF", "SIGWINCH",
		//	"SIGIO", "SIGPOLL", "SIGPWR", "SIGSYS", "SIGUNUSED",
		//	"HUP", "INT", "QUIT", "ILL", "TRAP", "ABRT", "IOT",
		//	"BUS", "FPE", "KILL", "USR1", "SEGV", "USR2", "PIPE",
		//	"ALRM", "TERM", "STKFLT", "CHLD", "CONT", "STOP",
		//	"TSTP", "TTIN", "TTOU", "URG", "XCPU", "XFSZ", "VTALRM",
		//	"PROF", "WINCH", "IO", "POLL", "PWR", "SYS", "UNUSED",
		//};

		//for (const auto& s : ss) {
		//	int n = sm::signal::has_signal(s.data(), (unsigned)s.size());
		//	std::cout << s << " -> " << n << std::endl;
		//}

		//if (ac > 1) {
		//
		//	char** argv = av + 1;
		//
		//	while (*argv != nullptr) {
		//
		//		int n = sm::signal::has_signal(*argv,
		//				(unsigned)strlen(*argv));
		//		std::cout << *argv << " -> \x1b[32m" << n << "\x1b[0m" << std::endl;
		//		++argv;
		//	}
		//
		//}
		//
		//return 0;


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
