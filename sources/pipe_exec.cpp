#include "pipe_exec.hpp"
#include "system/pipe.hpp"
#include "system/fcntl.hpp"
#include "string/strlen.hpp"
#include <unistd.h>


// -- P I P E -----------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::pipe_exec::pipe_exec(void)
/* : _pipefd{} not initialized */ {

	// create pipe
	sm::pipe(_fds);

	// get flags
	const int flags = sm::fcntl(_fds[1U], F_GETFD);

	// set flags
	sm::fcntl(_fds[1U], F_SETFD, flags | FD_CLOEXEC);
}

/* destructor */
sm::pipe_exec::~pipe_exec(void) noexcept {

	// close read end
	if (_fds[0U] != -1)
		static_cast<void>(::close(_fds[0U]));

	// close write end
	if (_fds[1U] != -1)
		static_cast<void>(::close(_fds[1U]));
}


// -- public methods ----------------------------------------------------------

/* read */
auto sm::pipe_exec::read(void) -> sm::isize {

	// close write end
	static_cast<void>(::close(_fds[1U]));
	_fds[1U] = -1;

	// read from pipe
	const auto bytes = ::read(_fds[0U], _buffer, sizeof(_buffer) - 1U);

	// check for error
	if (bytes == -1)
		throw sm::system_error("read");

	// null terminate
	_buffer[bytes] = '\0';

	// return bytes read
	return static_cast<sm::isize>(bytes);
}

/* write */
auto sm::pipe_exec::write(const char* str) -> void {

	// close read end
	static_cast<void>(::close(_fds[0U]));
	_fds[0U] = -1;

	// write to pipe
	const auto bytes = ::write(_fds[1U], str, sm::strlen(str));

	// check for error
	if (bytes == -1)
		throw sm::system_error("write");
}


// -- public accessors --------------------------------------------------------

/* data */
auto sm::pipe_exec::data(void) const noexcept -> const char* {
	return _buffer;
}
