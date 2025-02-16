#ifdef UNUSED_CODE

#include "other/daemon.hpp"
#include "log/logger.hpp"
#include "taskmaster.hpp"


// ╭───────────────────────────────────────╮
// │ le cube de johnny le taskmaster !     │
// ╰───────────────────────────────────────╯
#include <dirent.h>
#include "resources/unique_dir.hpp"
#include "system/open.hpp"
#include "reader.hpp"

#include <sys/mman.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#include "guards/lock_guard.hpp"
#include "guards/immutable_guard.hpp"

namespace sm {

struct rwx {
	static constexpr int value = 7;
};
struct r__ {
	static constexpr int value = 4;
};
struct rw_ {
	static constexpr int value = 6;
};
struct r_x {
	static constexpr int value = 5;
};
struct _wx {
	static constexpr int value = 3;
};
struct _w_ {
	static constexpr int value = 2;
};
struct __x {
	static constexpr int value = 1;
};
struct ___ {
	static constexpr int value = 0;
};

template <typename U = ___,
		  typename G = ___,
		  typename O = ___>
class permissions final {
public:
	static constexpr int value = (U::value << 6) | (G::value << 3) | O::value;
};
}


	//int fd = open("/proc/self/status", O_RDONLY);
	//
	//if (fd == -1) {
	//	perror("open");
	//	return EXIT_FAILURE;
	//}
	//
	//while (true) {
	//
	//	char buffer[4096];
	//	ssize_t size = read(fd, buffer, sizeof(buffer));
	//
	//	if (size == -1) {
	//		perror("read");
	//		close(fd);
	//		return EXIT_FAILURE;
	//	}
	//
	//	if (size == 0) {
	//		break;
	//	}
	//
	//	write(STDOUT_FILENO, buffer, (unsigned)size);
	//}
	//
	//return EXIT_SUCCESS;






int is_pointer_in_valid_range(void* ptr) {
	FILE* maps = fopen("/proc/self/maps", "r");
	if (maps == NULL) {
		perror("Erreur ouverture /proc/self/maps");
		return 0;
	}

	char line[256];
	unsigned long start, end;
	while (fgets(line, sizeof(line), maps)) {
		if (sscanf(line, "%lx-%lx", &start, &end) == 2) {
			if ((unsigned long)ptr >= start && (unsigned long)ptr < end) {
				fclose(maps);
				return 1; // Pointeur valide
			}
		}
	}

	fclose(maps);
	return 0; // Pointeur invalide
}

void check_argv_with_maps(int argc, char** argv) {
	for (int i = 0; i < argc; ++i) {
		if (argv[i] == NULL) {
			printf("argv[%d] est NULL.\n", i);
			continue;
		}

		if (is_pointer_in_valid_range(argv[i])) {
			printf("argv[%d] (%p) pointe sur une zone mémoire valide.\n", i, (void*)argv[i]);
		} else {
			printf("argv[%d] (%p) pointe sur une zone mémoire invalide !\n", i, (void*)argv[i]);
		}
	}
}

//sm::unique_fd fd = sm::open(".", O_RDWR | O_TMPFILE, 0666);

	//// minishell
	//try {
	//
	//	////test t1;
	//	//sm::unique_fd fd = sm::open(".yolo", O_RDWR, 0666);
	//	//sm::lock_guard lock{fd};
	//	//
	//	//auto pid = sm::fork();
	//	//
	//	//if (pid == 0) {
	//
	//
	//		//t1.print_addr();
	//		//char prog[] {"/bin/ls"};
	//		//char* args[] {prog, nullptr};
	//
	//		//if (::execv(prog, args) == -1)
	//		//	;
	//
	//		//tm.~taskmaster();
	//		//_exit(0);
	//
	//		//return 0;
	//
	//		exit(0);
	//		throw exit_exception{};
	//	}
	//	else {
	//
	//
	//		//t1.print_addr();
	//		waitpid(pid, nullptr, 0);
	//		sleep(5);
	//	}
	//}
	//catch (const exit_exception& e) {
	//	std::cout << "exit" << std::endl;
	//}
	//catch (const sm::exception& e) {
	//	std::cout << e.what() << std::endl;
	//}
	//
	//
	//return 0;




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








				// create directories
				//for (const auto& dir : _dirs) {
				//
				//	struct ::stat st;
				//
				//	// check if directory exists
				//	if (::stat(dir, &st) == -1) {
				//
				//		if (errno != ENOENT) {
				//			//perror("stat");
				//			return;
				//		}
				//
				//		// create directory
				//		if (::mkdir(dir, 0755) == -1) {
				//			//perror("mkdir");
				//			return;
				//		}
				//	}
				//
				//	// check if directory
				//	if (!S_ISDIR(st.st_mode)) {
				//		//perror("stat");
				//		return;
				//	}
				//}
				//
				//// open file
				//_fd = ::open(_path, O_CREAT | O_WRONLY, 0644);
				//
				//// check error
				//if (_fd == -1) {
				//	//perror("open");
				//	return;
				//}
				//
				//// remove file
				//if (::unlink(_path) == -1) {
				//	//perror("unlink");
				//	return;
				//}
				//
				//// get process id
				//auto pid = ::getpid();
				//
				////snprintf(_proc_path, PATH_MAX, "/proc/%d/fd/%d", pid, _fd);
				//
				//// create symlink
				//if (::symlink(_proc_path, _path) == -1) {
				//	//perror("symlink");
				//	return;
				//}
				//
				//::write(_fd, "hello, world!\n", 14);

				// ---------------------------------

				// get process path
				//__builtin_memcpy(_proc_path, "/proc/", 6U);

				//unsigned i = PATH_MAX;
				//
				//// convert pid to string
				//do {
				//
				//	_proc_path[--i] = static_cast<char>((pid % 10) ^ 48);
				//	pid /= 10;
				//
				//} while (pid != 0);
				//
				//__builtin_memcpy(_proc_path + 6U, _proc_path + i, PATH_MAX - i);

				//__builtin_memcpy(_proc_path + PATH_MAX - i, "/fd/1\0", 6U);

				//__builtin_memcpy(ptr, "/fd/1", 5U);

				//printf("proc path: %s\n", _proc_path);


				//if (_fd == -1)
				//	return;
				//
				//// remove symlink
				//if (::unlink(_path) == -1)
				//	perror("unlink");
				//
				//// open proc file
				//const int src = ::open(_proc_path, O_RDONLY);
				//
				//if (src == -1) {
				//	perror("src open");
				//	return;
				//}
				//
				//// open log file
				//const int dst = ::open(_path, O_WRONLY | O_CREAT, 0644);
				//
				//if (dst == -1) {
				//	perror("dst open");
				//	return;
				//}
				//
				//// copy proc contents into file
				//struct ::stat st;
				//
				//if (::fstat(src, &st) == -1) {
				//	perror("fstat");
				//	return;
				//}
				//
				//printf("st_size: %ld\n", st.st_size);
				//
				//const auto bytes = ::sendfile(dst, src, nullptr, (size_t)st.st_size);
				//
				//if (bytes == -1) {
				//	perror("sendfile");
				//	return;
				//}
				//
				//// close file
				//static_cast<void>(::close(src));
				//static_cast<void>(::close(dst));
#endif
