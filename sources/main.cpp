#include "syscall.hpp"
#include "controller.hpp"
#include "reader.hpp"
#include "write.hpp"
#include "dispatch.hpp"
#include "memory/malloc.hpp"
#include "process/wait_status.hpp"
#include "process/process_id.hpp"
#include "daemon.hpp"
#include "terminal.hpp"
#include "escape.hpp"
#include "running.hpp"
#include "readline.hpp"

#include <iostream>

#include <readline/readline.h>

#include "network/socket.hpp"
#include "network/addr.hpp"
#include "network/server.hpp"


namespace sm {



	// ╭───────────────────────────────────────╮
	// │ ls -la 'file.txt'         fiefjwoeijf │
	// ╰───────────────────────────────────────╯





} // namespace sm




template <bool>
auto termios_test(void) -> void;


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "unique_dir.hpp"


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

#include "process/process_event.hpp"

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


auto main(void) -> int {

	//list_processes();
	//return 0;


	try {

		sm::controller c{};

		c.run();

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
	}

	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
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
			throw std::runtime_error("read failed");

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
