#include "taskmaster/other/daemon.hpp"
#include "taskmaster/log/logger.hpp"
#include "taskmaster/taskmaster.hpp"


// ╭───────────────────────────────────────╮
// │ le cube de johnny le taskmaster !     │
// ╰───────────────────────────────────────╯
#include <dirent.h>
#include "common/resources/unique_dir.hpp"
#include "common/system/open.hpp"
#include "common/reader.hpp"

#include <sys/mman.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#include "common/guards/lock_guard.hpp"
#include "common/guards/immutable_guard.hpp"

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




auto list_processes(void) -> void {

	struct ::dirent* entry;

	sm::unique_dir proc{"/proc"};

	constexpr char line[] = "Name:\ttaskmaster\n";
	constexpr auto length = sizeof(line) - 1U;

	const ::pid_t process_id = ::getpid();

	start:
	// loop over entries
	while ((entry = proc.read()) != nullptr) {

		// skip non-directories
		if (entry->d_type != DT_DIR)
			continue;


		// declare pid
		::pid_t pid = 0;

		//// check for numeric directory name
		//for (unsigned i = 0U; i < sizeof(entry->d_name)
		//		|| entry->d_name[i] != '\0'; ++i) {
		//
		//	pid *= 10;
		//	pid += static_cast<::pid_t>(entry->d_name[i] - '0');
		//
		//	if (entry->d_name[i] < '0'
		//	 || entry->d_name[i] > '9') {
		//		goto start;
		//	}
		//}


		// vérifier si le nom du répertoire est un nombre (pid)
		if (not isdigit(entry->d_name[0U])) {
			continue;
		}

		// vérifier si le pid est le notre
		if (::atoi(entry->d_name) == pid) {
			continue;
		}

		char path[PATH_MAX];

		snprintf(path, PATH_MAX, "/proc/%s/status", entry->d_name);

		const sm::unique_fd fd = sm::open(path, O_RDONLY);

		sm::reader<length> reader;

		reader.read(fd);

		if (reader.size() == length
		 && strncmp(reader.data(), line, reader.size()) == 0)
			throw sm::runtime_error("taskmaster is already running");
	}

}


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

