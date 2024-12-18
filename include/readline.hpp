#ifndef readline_hpp
#define readline_hpp

#include "types.hpp"
#include "terminal/terminal.hpp"
#include "events/monitor.hpp"
#include "box.hpp"

#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R E A D L I N E -----------------------------------------------------

	class readline final : public sm::terminal::observer,
						   public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::readline;


			// -- private members ---------------------------------------------

			/* box */
			sm::box _box;

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
			auto _render(void) -> void;

			/* insert */
			auto _insert(const char&) -> void;

			/* move left */
			auto _move_left(void) -> void;

			/* move right */
			auto _move_right(void) -> void;

			/* delete */
			auto _delete(void) -> void;

			/* return */
			auto _return(sm::taskmaster&) -> void;


			/* debug */
			auto _debug(void) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			readline(void);

			/* destructor */
			~readline(void) noexcept;

			/* deleted copy constructor */
			readline(const self&) = delete;

			/* deleted move constructor */
			readline(self&&) = delete;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;

			/* on resize */
			auto on_resize(const unsigned short&,
						   const unsigned short& cols) noexcept -> void override;

	}; // class readline

} // namespace sm

#endif // readline_hpp
