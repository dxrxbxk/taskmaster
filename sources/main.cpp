#include "system/syscall.hpp"
#include "reader.hpp"
#include "system/write.hpp"
#include "dispatch.hpp"
#include "memory/malloc.hpp"
#include "process/wait_status.hpp"
#include "process/process_id.hpp"
#include "core/daemon.hpp"
#include "terminal.hpp"
#include "escape.hpp"
#include "running.hpp"
#include "readline.hpp"

#include <iostream>


namespace sm {



	// ╭───────────────────────────────────────╮
	// │ ls -la 'file.txt'         fiefjwoeijf │
	// ╰───────────────────────────────────────╯



	class box final {


		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = unsigned short;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::box;


			// -- private members ---------------------------------------------

			/* box */
			std::string _box;


			// -- private constants -------------------------------------------

			/* symbol enum */
			enum : unsigned {
				LINE_H, LINE_V,
				CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR,
				BORDER_MAX
			};

			/* symbols */
			static constexpr const char* _symbols[BORDER_MAX] = {
				"\xe2\x94\x80",
				"\xe2\x94\x82",
				"\xe2\x95\xad",
				"\xe2\x95\xae",
				"\xe2\x95\xb0",
				"\xe2\x95\xaf"
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			box(void) noexcept(noexcept(std::string{})) = default;

			/* copy constructor */
			box(const self&) = default;

			/* move constructor */
			box(self&&) noexcept = default;

			/* destructor */
			~box(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* set */
			auto set(const unsigned short& x,
					 const unsigned short& y,
					 const unsigned short& w,
					 const unsigned short& h) -> void {

				// avoid namespace pollution
				using esc = sm::escape;

				if (w == 0U || h == 0U)
					return;

				// clear previous box
				_box.clear();

				// append top left corner
				_box.append(_symbols[CORNER_TL], 3U);

				// append top box
				for (size_type i = 1U; i < w - 1U; ++i)
					_box.append(_symbols[LINE_H], 3U);

				// append top right corner
				_box.append(_symbols[CORNER_TR], 3U);

				// append move position bottom left corner
				_box.append(esc::move_position(x, y + h - 1));

				// append bottom left corner
				_box.append(_symbols[CORNER_BL], 3U);

				// append bottom box
				for (size_type i = 1U; i < w - 1U; ++i)
					_box.append(_symbols[LINE_H], 3U);

				// append bottom right corner
				_box.append(_symbols[CORNER_BR], 3U);


				size_type y_pos = y + h - 1U;

				// append left vertical box
				for (size_type i = y + 1U; i < y_pos; ++i) {
					// append move position left box
					_box.append(esc::move_position(x, i));
					// append left vertical box
					_box.append(_symbols[LINE_V], 3U);
				}

				size_type x_pos = x + w - 1U;

				// append right vertical box
				for (size_type i = y + 1U; i < y_pos; ++i) {
					// append move position right box
					_box.append(esc::move_position(x_pos, i));
					// append right vertical box
					_box.append(_symbols[LINE_V], 3U);
				}

			}

	}; // class box


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


auto main(void) -> int {

	//list_processes();
	//return 0;

	try {

		//termios_test<true>();
		//return 0;
		//

		//sm::wait_status status{};

		sm::readline rl{};


		sm::dispatch d;

		d.add(rl, EPOLLIN);

		while (sm::running::state()) {
			d.wait();
		}

		//sm::daemon daemon;
		//daemon.run();
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
