#include "common/system/pipe.hpp"
#include <unistd.h>


// -- P I P E -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::pipe::pipe(void)
/* : _pipefd{} not initialized */ {

	// create pipe
	if (::pipe(_fds) == -1)
		throw sm::system_error("pipe");
}

/* move constructor */
sm::pipe::pipe(self&& other) noexcept
: _fds{other._fds[0U],
		  other._fds[1U]} {

	// invalidate other
	other._fds[0U] = -1;
	other._fds[1U] = -1;
}

/* destructor */
sm::pipe::~pipe(void) noexcept {

	// close read end
	if (_fds[0U] != -1)
		static_cast<void>(::close(_fds[0U]));

	// close write end
	if (_fds[1U] != -1)
		static_cast<void>(::close(_fds[1U]));
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto sm::pipe::operator=(self&& other) noexcept -> self& {

	// check for self-assignment
	if (this == &other)
		return *this;

	// self destruct
	this->~pipe();

	// move
	_fds[0U] = other._fds[0U];
	_fds[1U] = other._fds[1U];

	// invalidate other
	other._fds[0U] = -1;
	other._fds[1U] = -1;

	return *this;
}


// -- public methods ----------------------------------------------------------

/* close read */
auto sm::pipe::close_read(void) noexcept -> void {

	// close read end
	if (_fds[0U] != -1) {
		static_cast<void>(::close(_fds[0U]));
		_fds[0U] = -1;
	}
}

/* close write */
auto sm::pipe::close_write(void) noexcept -> void {

	// close write end
	if (_fds[1U] != -1) {
		static_cast<void>(::close(_fds[1U]));
		_fds[1U] = -1;
	}
}
