#ifndef readline_hpp
#define readline_hpp

#include "common/types.hpp"
#include "taskcontrol/terminal.hpp"
#include "escape.hpp"
#include "io_event.hpp"
#include "common/reader.hpp"
#include "common/running.hpp"

#include <iostream>
#include <string>
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R E A D L I N E -----------------------------------------------------

	class readline final : public sm::terminal::observer {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::readline;


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


				//_buffer.append(_prompt);
				//
				//// input offset
				//const char* ptr = _input.data() + _offset;
				//
				//// size of substring
				//const sm::usize size = _input.size() > _term_width ? _term_width : _input.size();
				//
				//_buffer.append(ptr, size);
				//
				//_buffer.append("\r", 1U);
				//_buffer.append(sm::escape::move_right(static_cast<unsigned short>(_cursor_pos + 13U)));


				// move to middle of terminal
				_buffer.append(sm::escape::move_right(static_cast<unsigned short>(

								(_term_width / 2U) - (_input_pos)
								)));

				_buffer.append(_input);


				// |                       | (terminal)
				// ----------v (cursor)
				//           cmdline with a
				//     cmdline with args

				_buffer.append("\r");
				_buffer.append(sm::escape::move_right(static_cast<unsigned short>(

								(_term_width / 2U)
								)));


				::write(STDOUT_FILENO, _buffer.data(), _buffer.size());
			}



			/* insert */
			auto _insert(const char& c) -> void {

				_input.insert(_input_pos, 1U, c);

				++_input_pos;
				_render();
			}

			/* move left */
			auto _move_left(void) -> void {

				_input_pos -= (_input_pos > 0U);
				_render();
			}

			/* move right */
			auto _move_right(void) -> void {

				_input_pos += (_input_pos < _input.size());
				_render();
			}

			/* delete */
			auto _delete(void) -> void {

				if (_input_pos == 0U)
					return;

				_input.erase(--_input_pos, 1U);

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

			readline(void)
			: _prompt{}, _buffer{}, _input{},
			  _input_pos{0U}, _cursor_pos{0U}, _offset{0U},
			  _term_width{sm::terminal::width()} {

				// set terminal to raw mode
				sm::terminal::raw();
			}

			~readline(void) noexcept {

				// restore terminal
				sm::terminal::restore();
			}


			auto fd(void) const noexcept -> int /* override */ {
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

			auto on_event(const ::uint32_t& events) -> void /* override */ {

				//if (events & EPOLLIN) {

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
				//}

			}

	}; // class readline

} // namespace sm

#include <readline/readline.h>

namespace sm {


	class readline2 final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::readline2;


			// -- private members ---------------------------------------------

			/* line */
			std::string _line;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			readline2(void)
			: _line{} {

				// disable readline completion
				::rl_bind_key('\t', ::rl_insert);
			}


			// -- public methods ----------------------------------------------

			/* read */
			auto read(void) -> void {

				char* line = ::readline("taskcontrol> ");

				if (line == nullptr)
					return;

				try { _line = line; }
				catch (...) {}

				::free(line);
			}


			// -- public accessors --------------------------------------------

			/* line */
			auto line(void) const noexcept -> const std::string& {
				return _line;
			}

	}; // class readline2

} // namespace sm

#endif // readline_hpp
