#ifndef readline_hpp
#define readline_hpp

#include "types.hpp"
#include "terminal/terminal.hpp"
#include "terminal/escape.hpp"
#include "io_event.hpp"
#include "reader.hpp"
#include "events/monitor.hpp"

#include <iostream>
#include <string>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R E A D L I N E -----------------------------------------------------

	class readline final : public sm::terminal::observer, public sm::listener {


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

				//_debug();
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
			auto _return(sm::taskmaster&) -> void;


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


			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;

	}; // class readline

} // namespace sm

#include <readline/readline.h>




namespace sm {


	class readline2 final : public sm::listener {


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

				// load history
				if (::read_history(".taskmaster_history") != 0)
					throw sm::system_error("read_history");

				::rl_callback_handler_install("", &self::_line_handler);
				::rl_set_prompt("taskmaster > ");
				::rl_redisplay();

				rl_catch_sigwinch = 1;

				// disable readline completion
				//::rl_bind_key('\t', ::rl_insert);
			}

			/* destructor */
			~readline2(void) noexcept {

				// remove handler
				::rl_callback_handler_remove();

				// save to history
				static_cast<void>(::write_history(".taskmaster_history"));
			}


			// -- public accessors --------------------------------------------

			/* line */
			auto line(void) const noexcept -> const std::string& {
				return _line;
			}


		private:


			static auto _line_handler(char *line) -> void {

				if (line == nullptr) {
					printf("EOF received, exiting...\n");
					::rl_callback_handler_remove();
					exit(0);
				}

				if (*line) {
					::add_history(line);
				}


				printf("Line received: %s\n", line);
				::free(line);
			}


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return STDIN_FILENO;
			}

			/* on event */
			auto on_event(const sm::event& events, sm::taskmaster& tm) -> void override {

				// ::rl_resize_terminal();

				if (events.is_in()) {
					::rl_callback_read_char();
				}
			}



	}; // class readline2

} // namespace sm

#endif // readline_hpp
