#ifndef readline2_hpp
#define readline2_hpp

#include <readline/readline.h>
#include <readline/history.h>
#include "events/monitor.hpp"
#include "diagnostics/exception.hpp"

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

#endif // readline2_hpp
