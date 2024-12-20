#include "terminal/terminal.hpp"
#include "diagnostics/exception.hpp"
#include "log/logger.hpp"

#include <signal.h>
#include <unistd.h>

#include <iostream>


// -- T E R M I N A L ---------------------------------------------------------

// -- private static methods --------------------------------------------------

/* get */
auto sm::terminal::_get(void) -> struct ::termios {

	struct ::termios term;

	// get terminal attributes
	if (::tcgetattr(STDIN_FILENO, &term) == -1)
		throw sm::system_error{"tcgetattr"};

	return term;
}

/* set */
auto sm::terminal::_set(const struct ::termios& term) -> void {

	// set terminal attributes
	if (::tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		throw sm::system_error{"tcsetattr"};
}

/* setup raw */
auto sm::terminal::_setup_raw(struct ::termios& term) noexcept -> void {

	// disable input flags
	term.c_iflag &= ~static_cast<::tcflag_t>(
		IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON
	);

	// disable output flags
	term.c_oflag &= ~static_cast<::tcflag_t>(
		OPOST
	);

	// disable local flags
	term.c_lflag &= ~static_cast<::tcflag_t>(
		ECHO | ECHONL | ICANON | ISIG | IEXTEN
	);

	// disable control flags
	term.c_cflag &= ~static_cast<::tcflag_t>(
		CSIZE | PARENB
	);

	// enable 8-bit characters
	term.c_cflag |= static_cast<::tcflag_t>(
		CS8
	);

	// set minimum number of characters for non-canonical read
	term.c_cc[VMIN] = 1U;

	// set timeout in deciseconds for non-canonical read
	term.c_cc[VTIME] = 0U;
}

/* shared */
auto sm::terminal::_shared(void) -> self& {
	static self instance;
	return instance;
}

/* resize handler */
auto sm::terminal::_resize_handler(int) noexcept -> void {

	// get instance
	auto& instance = self::_shared();

	const auto status = ::ioctl(STDIN_FILENO, TIOCGWINSZ, &instance._size);

	if (status == -1)
		return;

	// loop over observers
	for (const auto observer : self::_shared()._observers)
		observer->on_resize(instance._size.ws_row,
							instance._size.ws_col);
}


// -- private lifecycle -------------------------------------------------------

/* default constructor */
sm::terminal::terminal(void)
: _pid{::getpid()}, _origin{self::_get()}, _raw{_origin}, _observers{} {

	// setup raw mode
	self::_setup_raw(_raw);

	// add resize handler
	if (::signal(SIGWINCH, self::_resize_handler) == SIG_ERR)
		throw sm::system_error("signal sigwinch");

	// get terminal size
	if (::ioctl(STDIN_FILENO, TIOCGWINSZ, &_size) == -1)
		throw sm::system_error("ioctl tiocgwinsz");
}

/* destructor */
sm::terminal::~terminal(void) noexcept {

	if (_pid != ::getpid())
		return;

	// restore terminal attributes
	static_cast<void>(::tcsetattr(STDIN_FILENO, TCSANOW, &_origin));
}


// -- public static methods ---------------------------------------------------

/* raw */
auto sm::terminal::raw(void) -> void {
	self::_set(self::_shared()._raw);
}

/* restore */
auto sm::terminal::restore(void) -> void {
	self::_set(self::_shared()._origin);
}

/* subscribe */
auto sm::terminal::subscribe(sm::terminal::observer& obs) -> void {
	self::_shared()._observers.push_back(&obs);
}

/* unsubscribe */
auto sm::terminal::unsubscribe(sm::terminal::observer& obs) noexcept -> void {

	auto& observers = self::_shared()._observers;

	for (auto it = observers.begin(); it != observers.end(); ++it) {
		if (*it == &obs) {
			observers.erase(it);
			return; }
	}
}

/* width */
auto sm::terminal::width(void) noexcept -> unsigned short {
	return _shared()._size.ws_col;
}

/* height */
auto sm::terminal::height(void) noexcept -> unsigned short {
	return _shared()._size.ws_row;
}




