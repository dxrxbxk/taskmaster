#include "resources/unique_fd.hpp"
#include "diagnostics/exception.hpp"

#include <unistd.h>


// -- U N I Q U E  F D --------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::unique_fd::unique_fd(void) noexcept
: _fd{-1} {
}

/* int constructor */
sm::unique_fd::unique_fd(const int& fd) noexcept
: _fd{fd} {
}

/* move constructor */
sm::unique_fd::unique_fd(self&& other) noexcept
: _fd{other._fd} {

	// invalidate other
	other._fd = -1;
}

/* destructor */
sm::unique_fd::~unique_fd(void) noexcept {

	if (_fd == -1)
		return;

	// close file descriptor
	static_cast<void>(::close(_fd));
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto sm::unique_fd::operator=(self&& other) noexcept -> self& {

	// check for self assignment
	if (this == &other)
		return *this;

	// close file descriptor
	if (_fd != -1)
		static_cast<void>(::close(_fd));

	// move file descriptor
	_fd = other._fd;

	// invalidate other
	other._fd = -1;

	return *this;
}


// -- public conversion operators ---------------------------------------------

/* int conversion operator */
sm::unique_fd::operator const int&(void) const noexcept {
	return _fd;
}

/* bool conversion operator */
sm::unique_fd::operator bool(void) const noexcept {
	return _fd != -1;
}


// -- public operators --------------------------------------------------------

/* not operator */
auto sm::unique_fd::operator!(void) const noexcept -> bool {
	return _fd == -1;
}


// -- public methods ----------------------------------------------------------

/* close */
auto sm::unique_fd::close(void) -> void {

	// close file descriptor
	if (::close(_fd) == -1)
		throw sm::system_error("close");

	// invalidate file descriptor
	_fd = -1;
}
