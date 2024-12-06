#include "system/syscall.hpp"
#include "system/write.hpp"
#include "dispatch.hpp"
#include "memory/malloc.hpp"
#include "process/wait_status.hpp"
#include "process/process_id.hpp"
#include "core/daemon.hpp"
#include "terminal.hpp"
#include "escape.hpp"

#include <iostream>


namespace sm {


	template <unsigned int N>
	class reader final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::reader<N>;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[N];

			/* size */
			sm::usize _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			reader(void) noexcept
			: /* _buffer{} uninitialized */ _size{0U} {
			}

			/* copy constructor */
			reader(const self&) noexcept = default;

			/* move constructor */
			reader(self&&) noexcept = default;

			/* destructor */
			~reader(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char* {
				return _buffer;
			}

			/* data */
			auto data(void) const noexcept -> const char* {
				return _buffer;
			}

			/* size */
			auto size(void) const noexcept -> const sm::usize& {
				return _size;
			}

			/* operator[] */
			auto operator[](const sm::isize& i) noexcept -> char& {
				return _buffer[i];
			}

			/* operator[] */
			auto operator[](const sm::isize& i) const noexcept -> const char& {
				return _buffer[i];
			}


			// -- public methods ----------------------------------------------

			/* read */
			auto read(const int& fd) -> void {

				const auto bytes = ::read(fd, _buffer, N);

				if (bytes == -1)
					throw std::runtime_error("read failed");

				_size = static_cast<sm::usize>(bytes);
			}

	}; // class reader

			// ╭───────────────────────────────────────╮
			// │ ls -la 'file.txt'         fiefjwoeijf │
			// ╰───────────────────────────────────────╯


	class box final : public sm::terminal::observer {


		private:

			std::string _lines[3U];

			/* symbol enum */
			enum {
				LINE_H, LINE_V,
				CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR,
				BORDER_MAX
			};

			static constexpr const char* _symbols[BORDER_MAX] = {
				"\xe2\x94\x80",
				"\xe2\x94\x82",
				"\xe2\x95\xad",
				"\xe2\x95\xae",
				"\xe2\x95\xb0",
				"\xe2\x95\xaf"
			};



		public:

			// -- public overrides --------------------------------------------

			/* on resize */
			auto on_resize(const unsigned short& rows,
						   const unsigned short& cols) noexcept -> void override {

			}

	}; // class box


	class input_event final : public sm::io_event,
							  public sm::terminal::observer {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::input_event;


			// -- private members ---------------------------------------------

			/* buffer */
			std::string _buffer;

			/* input */
			std::string _input;

			/* position */
			sm::usize _cursor_pos;

			/* offset */
			sm::usize _offset;

			/* width */
			sm::usize _term_width;


			// -- private methods ---------------------------------------------

			/* render */
			auto _render(void) -> void {

				_buffer.clear();
				// erase line
				_buffer.append("\r\x1b[2K");

				_buffer.append(_input);

				// get substring from offset to offset + term_width
				//const auto substr = _input.substr(_offset, _term_width);

				// prompt
				//_buffer.append(substr);
				// move to cursor position

				//const auto esc = sm::escape::move_left

				//_buffer.append(sm::escape::move_left((unsigned short)(_input.size() - _cursor_pos)));


				/*
				| width term |
		  jfhelfeifeifjeijfij<
		 |offset|
		 */


				if (_cursor_pos - _offset >= _term_width) {
					_buffer.append(sm::escape::disable_cursor());
				}
				else {
					_buffer.append(sm::escape::enable_cursor());
				}

				::write(STDOUT_FILENO, _buffer.data(), _buffer.size());
			}



			/* insert */
			auto _insert(const char& c) -> void {

				_input.insert(_cursor_pos, 1U, c);
				if (_cursor_pos < _term_width) {
					++_cursor_pos;
				}
				//++_cursor_pos;

				//if (_cursor_pos >= _term_width) {
				//	_offset = _cursor_pos - _term_width;
				//}

				_render();
			}

			/* move left */
			auto _move_left(void) -> void {

				if (_cursor_pos == 0U)
					return;

				--_cursor_pos;
				_render();
			}

			/* move right */
			auto _move_right(void) -> void {

				if (_cursor_pos == _input.size())
					return;

				++_cursor_pos;
				_render();
			}

			/* delete */
			auto _delete(void) -> void {

				if (_cursor_pos == 0U)
					return;

				_input.erase(--_cursor_pos, 1U);
				_render();
			}

			/* return */
			auto _return(void) -> void {

				::write(STDOUT_FILENO, "\r\n", 2U);

				_input.clear();
				_cursor_pos = 0U;
			}



		public:

			input_event(void)
			: _buffer{}, _input{}, _cursor_pos{0U}, _offset{0U},
			  _term_width{sm::terminal::width()} {

				// set terminal to raw mode
				sm::terminal::raw();
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

				_term_width = cols;

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
							exit(0);
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


auto main(void) -> int {

	try {

		//termios_test<true>();
		//return 0;
		//

		//sm::wait_status status{};


		sm::input_event ie{};

		sm::dispatch d;

		d.add(ie, EPOLLIN);

		while (true) {
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
