#include "readline.hpp"
#include "taskmaster.hpp"
#include "string/split.hpp"
#include "terminal/escape.hpp"
#include "reader.hpp"
#include <unistd.h>


// -- R E A D L I N E ---------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::readline::readline(void)
: _prompt{}, _buffer{}, _input{},
  _input_pos{0U}, _cursor_pos{0U}, _offset{0U},
  _term_width{sm::terminal::width() - 13U} {

	// set terminal to raw mode
	sm::terminal::raw();
	//_box.set();
	//_box.print();
	//// move up
	//::write(STDOUT_FILENO, "\x1b[1A", 4U);
	_render();
}

/* destructor */
sm::readline::~readline(void) noexcept {

	// restore terminal
	sm::terminal::restore();
}



// -- public overrides --------------------------------------------------------

/* fd */
auto sm::readline::fd(void) const noexcept -> int {
	return STDIN_FILENO;
}

/* on event */
auto sm::readline::on_event(const sm::event& events, sm::taskmaster& tm) -> void {

	if (events.is_err())
		throw sm::runtime_error("readline: error event");

	if (not events.is_in())
		throw sm::runtime_error("readline: not an input event");

	// read input
	sm::reader<128U> reader{};
	reader.read(STDIN_FILENO);


	if (reader.size() == 1U) {

		// ctrl-c / ctrl-d
		if (reader[0U] == '\x03' || reader[0U] == '\x04') {
			::write(STDOUT_FILENO, "\r\x1b[2K", 5U);
			tm.stop();
			return;
		}

		// tab
		else if (reader[0U] == '\t') {
			// _completion();
			return;
		}

		// delete
		else if (reader[0U] == '\x7f') {
			self::_delete();
			return;
		}

		// return
		else if (reader[0U] == '\r') {
			self::_return(tm);
			return;
		}

		// unexpected control character
		else if (reader[0U] < ' ') {
			return;
		}

		// insert character
		else {
			self::_insert(reader[0U]);
			return;
		}
	}


	else if (reader.size() == 3U) {

		if (reader[0U] == '\x1b' && reader[1U] == '[') {

			// move cursor right
			if (reader[2U] == 'C')
				self::_move_right();

			// move cursor left
			else if (reader[2U] == 'D')
				self::_move_left();

		}
	}

}


// -- private methods ---------------------------------------------------------

/* render */
auto sm::readline::_render(void) -> void {

	_buffer.clear();

	// erase line
	_buffer.append("\r\x1b[2K\x1b[32mtaskmaster > \x1b[0m");


	// Calcul de la portion visible
    const char* ptr = _input.data() + _offset;
    const sm::usize visible_size = std::min(_term_width, _input.size() - _offset);

    // Ajouter la portion visible au buffer
    _buffer.append(ptr, visible_size);

    // Replacer le curseur
    _buffer.append("\r");
    const sm::usize visible_cursor_pos = _input_pos - _offset;

    if (visible_cursor_pos < _term_width) {
        _buffer.append(sm::escape::move_right(
					static_cast<unsigned short>(visible_cursor_pos + 13U)));
    }

    ::write(STDOUT_FILENO, _buffer.data(), _buffer.size());
}


/* insert */
auto sm::readline::_insert(const char& c) -> void {

	_input.insert(_input_pos, 1U, c);
    ++_input_pos;

    if (_cursor_pos < _term_width) {
        ++_cursor_pos;
    } else {
        ++_offset;
    }

    // Synchronisation explicite
    if (_cursor_pos + _offset != _input_pos) {
        _cursor_pos = _input_pos - _offset;
    }

    _render();
}

/* move left */
auto sm::readline::_move_left(void) -> void {

	if (_input_pos > 0) {
        --_input_pos;

        if (_cursor_pos == 0) {
            if (_offset > 0) {
                --_offset;
            }
        } else {
            --_cursor_pos;
        }

        // Synchronisation explicite
        if (_cursor_pos + _offset != _input_pos) {
            _cursor_pos = _input_pos - _offset;
        }
    }

    _render();
}

/* move right */
auto sm::readline::_move_right(void) -> void {

	if (_input_pos < _input.size()) {
        ++_input_pos;

        if (_cursor_pos < _term_width - 1) {
            ++_cursor_pos;
        } else {
            ++_offset;
        }

        // Synchronisation explicite
        if (_cursor_pos + _offset != _input_pos) {
            _cursor_pos = _input_pos - _offset;
        }
    }

    _render();
}

/* delete */
auto sm::readline::_delete(void) -> void {

	if (_input_pos == 0U)
		return;

	_input.erase(--_input_pos, 1U);

	_render();
}

/* return */
auto sm::readline::_return(sm::taskmaster& tm) -> void {

	::write(STDOUT_FILENO, "\r\n", 2U);

	if (not _input.empty()) {

		// split line by space
		auto argv = sm::split(_input, " \t\v\f");

		// execute command
		tm.executor().execute(tm, std::move(argv));

		_input.clear();
		_cursor_pos = 0U;
		_input_pos = 0U;
		_offset = 0U;
	}

	if (tm.is_running())
		_render();
}

#include "resources/unique_fd.hpp"

/* debug */
auto sm::readline::_debug(void) -> void {

	sm::unique_fd fd{::open("./readline.log", O_WRONLY | O_CREAT | O_TRUNC, 0644)};

	if (fd == -1)
		throw sm::system_error("open (readline.log)");

	::dprintf(fd, "input size: %zu\r\n", _input.size());
	::dprintf(fd, "input  pos: %zu\r\n", _input_pos);
	::dprintf(fd, "cursor pos: %zu\r\n", _cursor_pos);
	::dprintf(fd, "    offset: %zu\r\n", _offset);
}



/* on resize */
auto sm::readline::on_resize(const unsigned short& width,
							 const unsigned short& height) noexcept -> void {

	_term_width = width - 13U;
	//_term_width = 10U;

	// compute new offset

	return;
	if (_input_pos < _offset) {
		_offset = _input_pos; // Ajuste pour afficher la position actuelle du curseur
	} else if (_input_pos >= _offset + _term_width) {
		_offset = _input_pos - _term_width + 1;
	}

	_render();
}
