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


	class input_event final : public sm::io_event,
							  public sm::terminal::observer {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::input_event;


			// -- private members ---------------------------------------------

			/* prompt */
			std::string _prompt;

			/* buffer */
			std::string _buffer;

			/* input */
			std::string _input;

			/* input position */
			sm::usize _input_pos;

			/* position */
			sm::usize _cursor_pos;

			/* offset */
			sm::usize _offset;

			/* width */
			sm::usize _term_width;


			// -- private methods ---------------------------------------------

			/* render */
			auto _render(void) -> void {

				_debug();

				_buffer.clear();
				// erase line
				_buffer.append("\r\x1b[2K");
				_buffer.append(_prompt);

				// input offset
				const char* ptr = _input.data() + _offset;

				// size of substring
				const sm::usize size = _input.size() > _term_width ? _term_width : _input.size();

				_buffer.append(ptr, size);

				_buffer.append("\r", 1U);
				_buffer.append(sm::escape::move_right(static_cast<unsigned short>(_cursor_pos + 13U)));


				::write(STDOUT_FILENO, _buffer.data(), _buffer.size());
			}



			/* insert */
			auto _insert(const char& c) -> void {

				_input.insert(_input_pos, 1U, c);

				++_input_pos;

				if (_cursor_pos < _term_width)
					++_cursor_pos;
				else
					++_offset;

				_render();
			}


			/* move left */
			auto _move_left(void) -> void {

				_input_pos -= (_input_pos > 0U);

				if (_cursor_pos == 0U) {

					_offset -= (_offset != 0U);
				}

				else {
					--_cursor_pos;
				}

				_render();
			}

			/* move right */
			auto _move_right(void) -> void {

				if (_input_pos >= _input.size()) {
					return;
				}

				++_input_pos;

				if (_cursor_pos < _term_width - 1) {
					// Le curseur peut encore avancer dans la fenêtre visible
					++_cursor_pos;
				} else {
					// Le curseur est au bord droit, il faut scroller
					++_offset;
				}
				_render();
				return;

				//_input_pos += (_input_pos < _input.size());
				//
				//if (_cursor_pos < _term_width) {
				//	++_cursor_pos;
				//}
				//else {
				//
				//	if (_input_pos < _input.size())
				//		++_offset;
				//}
				//
				//_render();
			}

			/* delete */
			auto _delete(void) -> void {

				if (_input_pos == 0U)
					return;

				_input.erase(--_input_pos, 1U);

				if (_cursor_pos == 0U) {
					_offset -= (_offset != 0U);
				}
				else {
					--_cursor_pos;
				}

				_render();
			}

			/* return */
			auto _return(void) -> void {

				::write(STDOUT_FILENO, "\r\n", 2U);

				_input.clear();
				_cursor_pos = 0U;
				_input_pos = 0U;
				_offset = 0U;
			}

			auto _debug(void) -> void {

				// move home
				::write(STDOUT_FILENO, "\x1b[H", 3U);
				// clear screen
				::write(STDOUT_FILENO, "\x1b[2J", 4U);

				std::cout << "input size: " << _input.size() << "\r\n";
				std::cout << "input  pos: " << _input_pos << "\r\n";
				std::cout << "cursor pos: " << _cursor_pos << "\r\n";
				std::cout << "    offset: " << _offset << "\r\n";

				// move to bottom of terminal
				::write(STDOUT_FILENO, "\x1b[999B", 6U);
			}


		public:

			input_event(void)
			: _prompt{}, _buffer{}, _input{}, _input_pos{0U}, _cursor_pos{0U}, _offset{0U},
			  _term_width{sm::terminal::width() - 1U - 13U} {
			  //_term_width{10U} {

				// set terminal to raw mode
				sm::terminal::raw();

				_prompt.append("\x1b[32mtaskmaster > \x1b[0m");
				//_box.set
			}

			~input_event(void) noexcept {

				// restore terminal
				sm::terminal::restore();
			}


			auto fd(void) const noexcept -> int override {
				return STDIN_FILENO;
			}

			auto on_resize(const unsigned short&,
						   const unsigned short& cols) noexcept -> void override {

				_term_width = cols - 1U - 13U;
				//_term_width = 10U;

				// compute new offset
				_offset = _cursor_pos > _term_width ? _cursor_pos - _term_width : 0U;

				_render();
			}

			auto on_event(const ::uint32_t& events) -> void override {

				if (events & EPOLLIN) {

					sm::reader<128U> reader{};

					reader.read(STDIN_FILENO);

					if (reader.size() == 1U) {

						if (reader[0U] == 'q') {
							sm::running::stop();
						}

						// delete
						else if (reader[0U] == 127) {
							_delete();
						}
						else if (reader[0U] == '\r') {
							_return();
						}
						else {
							// insert character
							_insert(reader[0U]);
						}
					}
					else if (reader.size() == 3U) {

						if (reader[0U] == '\x1b' && reader[1U] == '[') {

							// move cursor right
							if (reader[2U] == 'C') {
								_move_right();
							}
							// move cursor left
							else if (reader[2U] == 'D') {
								_move_left();
							}
						}

					}
				}

			}
	};

} // namespace sm




template <bool>
auto termios_test(void) -> void;


#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


namespace sm {


	class unique_dir final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::unique_dir;


			// -- private members ---------------------------------------------

			/* directory */
			::DIR* _dir;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			unique_dir(void) noexcept
			: _dir{nullptr} {
			}

			/* path constructor */
			template <unsigned int N>
			unique_dir(const char (&path)[N])
			: _dir{::opendir(path)} {

				if (_dir == nullptr)
					throw std::runtime_error("opendir failed");
			}

			/* deleted copy constructor */
			unique_dir(const self&) = delete;

			/* move constructor */
			unique_dir(self&& other) noexcept
			: _dir{other._dir} {
				other._dir = nullptr;
			}

			/* destructor */
			~unique_dir(void) noexcept {

				if (_dir == nullptr)
					return;

				::closedir(_dir);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				if (this == &other)
					return *this;

				if (_dir != nullptr)
					::closedir(_dir);

				_dir = other._dir;
				other._dir = nullptr;

				return *this;
			}


			// -- public methods ----------------------------------------------

			/* read */
			auto read(void) -> ::dirent* {
				return ::readdir(_dir);
			}


	}; // class directory

} // namespace sm


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

	list_processes();
	return 0;

	try {

		//termios_test<true>();
		//return 0;
		//

		//sm::wait_status status{};

		//sm::readline rl{};

		sm::input_event ie{};

		sm::dispatch d;

		d.add(ie, EPOLLIN);

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
