#include "common/system/syscall.hpp"
#include "taskmaster/controller.hpp"
#include "common/reader.hpp"
#include "common/system/write.hpp"
#include "common/dispatch.hpp"
#include "common/memory/malloc.hpp"
#include "taskmaster/process/wait_status.hpp"
#include "taskmaster/process/process_id.hpp"
#include "taskmaster/daemon.hpp"
#include "taskcontrol/terminal.hpp"
#include "taskcontrol/escape.hpp"
#include "running.hpp"
#include "taskcontrol/readline.hpp"
#include "common/signal.hpp"
#include "common/diagnostics/exception.hpp"

#include <sys/mman.h>
#include "taskmaster/timestamp.hpp"
#include "taskmaster/logger.hpp"

#include <iostream>

#include <readline/readline.h>

#include "common/network/socket.hpp"
#include "common/network/addr.hpp"
#include "taskmaster/server.hpp"

#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "common/resources/unique_dir.hpp"



// ╭───────────────────────────────────────╮
// │ le cube de johnny le taskmaster !     │
// ╰───────────────────────────────────────╯



template <bool>
auto termios_test(void) -> void;



auto list_processes(void) -> void {

	struct ::dirent* entry;

	sm::unique_dir proc{"/proc"};

	while ((entry = proc.read()) != nullptr) {

		// Vérifier si le nom du répertoire est un nombre (PID)
		if (not isdigit(entry->d_name[0])) {
			continue;
		}

		char path[256];
		char name[256];
		snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);

		FILE *status_file = fopen(path, "r");
		if (status_file == NULL) {
			continue;
		}

		while (fgets(name, sizeof(name), status_file)) {
			if (strncmp(name, "Name:", 5) == 0) {
				// Extraire le nom du processus
				char process_name[256];
				sscanf(name, "Name:\t%255s", process_name);
				printf("%-10s %-30s\n", entry->d_name, process_name);
				break;
			}
		}
		fclose(status_file);
	}

}

#include "taskmaster/process/process_event.hpp"

namespace sm {


	class process_manager final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process_manager;


			// -- private members ---------------------------------------------

			/* processes */
			std::vector<sm::process> _processes;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_manager(void) = default;


			// -- public modifiers --------------------------------------------

			/* add */
			auto add(const sm::process_id& id) -> sm::process& {
				_processes.emplace_back(id);
				return _processes.back();
			}

			/* add */
			auto add(sm::process&& pro) -> sm::process& {
				_processes.push_back(static_cast<sm::process&&>(pro));
				return _processes.back();
			}



	}; // class process_manager

} // namespace sm

#include <fcntl.h>


namespace sm {

	class executor final {

		public:

			auto exec(void) -> void {

				// fork
				const auto pid = sm::fork();

				// parent process
				if (pid != 0)
					return;

				// child process

				// redirect stdout
				int fd = ::open("./log.txt", O_CREAT | O_WRONLY, 0644);

				if (fd == -1)
					throw sm::system_error("open");

				auto ret = ::dup2(fd, STDOUT_FILENO);

			}

	}; // class executor



} // namespace sm



auto main(void) -> int {


	try {

		sm::controller c{};
		c.run();
	}

	catch (const sm::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}



template <bool raw>
auto termios_test(void) -> void {

	if constexpr (raw == true) {
		sm::terminal::raw();
	}
	else {
		sm::terminal::restore();
	}


	std::cout << "\x1b[33mpress 'q' to quit\r\n\x1b[0m";

	while (true) {

		char buffer[32U];

		const auto readed = ::read(STDIN_FILENO, buffer, sizeof(buffer));

		if (readed == -1)
			throw sm::system_error("read");

		for (::ssize_t i = 0; i < readed; ++i) {

			if (buffer[i] == 'q')
				return;

			if (std::isprint(buffer[i]))
				std::cout << "\x1b[32m" << buffer[i] << "\x1b[0m";
			else
				std::cout << "\x1b[31m'\\x" << std::hex << static_cast<int>(buffer[i])
					<< std::dec << "'\x1b[0m ";
		}

		std::cout << std::flush;
		//std::cout << "\r\n";
	}
}
	//std::cout << ::getpid() << std::endl;

	//sm::logger log{};
	//return 0;

	/*
	std::cout << sm::timestamp{}.data() << std::endl;
	return 0;


	int fdm = memfd_create("my_memfile", 0);
	if (fdm == -1) {
		perror("memfd_create");
		return 1;
	}

	// Écrire dans le fichier en mémoire
	write(fdm, "Hello, memfd!\n", 14);

	// Lire depuis le fichier
	lseek(fdm, 0, SEEK_SET);
	char buf[64];
	read(fdm, buf, sizeof(buf));
	printf("Read: %s", buf);

	close(fdm);
	return 0;
	*/


	//list_processes();
	//return 0;

	/*
	std::cout << ::getpid() << std::endl;

	int fd = ::open("./log.txt", O_CREAT | O_WRONLY, 0644);
	if (fd == -1) {
		perror("open");
		return EXIT_FAILURE;
	}

	auto ret = ::dup2(fd, STDOUT_FILENO);
	if (ret == -1) {
		perror("dup2");
		return EXIT_FAILURE;
	}

	::close(fd);



	int i = 0;

	while (true) {

		char buffer[32U];

		int value = i;
		unsigned int j = 32U;
		buffer[--j] = '\n';

		do {
			buffer[--j] = '0' + (value % 10);
		} while (value /= 10);

		const char* ptr = buffer + j;

		if (::write(STDOUT_FILENO, "hello world ", 12) != 12) {
			perror("write");
			return EXIT_FAILURE;
		}

		if (::write(STDOUT_FILENO, ptr, 32U - j) != 32U - j) {
			perror("write");
			return EXIT_FAILURE;
		}


		::sleep(1);
		++i;
	}


	return 0;
	*/

		//termios_test<true>();
		//return 0;

		//sm::wait_status status{};

		//sm::readline rl{};
		//
		//sm::dispatch<int> d;
		//
		//d.add(rl, EPOLLIN);
		//

		/*
		sm::process_manager pm{};

		char program[] = "./program";
		char* argv[] = {program, nullptr};

		sm::daemon daemon;
		sm::process_id id = daemon.run(argv);

		auto& last = pm.add(id);

		d.add(last, EPOLLIN);

		int i = 0;
		while (sm::running::state()) {
			d.wait(i);
		}
		*/

		//d.add
