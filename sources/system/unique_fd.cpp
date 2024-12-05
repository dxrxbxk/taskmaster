#include "system/unique_fd.hpp"


// -- U N I Q U E  F D --------------------------------------------------------


// -- public lifecycle --------------------------------------------------------

/* default constructor */
ft::unique_fd::unique_fd(void) noexcept
: _fd{-1} {
}

/* int constructor */
ft::unique_fd::unique_fd(const int& fd) noexcept
: _fd{fd} {
}

/* move constructor */
ft::unique_fd::unique_fd(self&& other) noexcept
: _fd{other._fd} {

	// invalidate other
	other._fd = -1;
}

/* destructor */
ft::unique_fd::~unique_fd(void) noexcept {

	if (_fd == -1)
		return;

	// close file descriptor
	static_cast<void>(::close(_fd));
}


// -- public assignment operators ---------------------------------------------

/* move assignment operator */
auto ft::unique_fd::operator=(self&& other) noexcept -> self& {

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
ft::unique_fd::operator const int&(void) const noexcept {
	return _fd;
}

/* bool conversion operator */
ft::unique_fd::operator bool(void) const noexcept {
	return _fd != -1;
}


// -- public operators --------------------------------------------------------

/* not operator */
auto ft::unique_fd::operator!(void) const noexcept -> bool {
	return _fd == -1;
}


// -- public methods ----------------------------------------------------------

/* close */
auto ft::unique_fd::close(void) noexcept -> void {

	// check file descriptor
	if (_fd == -1)
		return;

	// close file descriptor
	static_cast<void>(::close(_fd));

	// invalidate file descriptor
	_fd = -1;
}
