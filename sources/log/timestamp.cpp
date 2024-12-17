#include "log/timestamp.hpp"
#include <ctime>


// -- T I M E S T A M P -------------------------------------------------------

/* default constructor */
sm::timestamp::timestamp(void) noexcept
: _buffer{
	'[',
	'D', 'D',
	'/',
	'M', 'M',
	'/',
	'Y', 'Y', 'Y', 'Y',
	'-',
	'H', 'H',
	':',
	'M', 'M',
	':',
	'S', 'S',
	']',
	'\0'
} {

	// get time
	const ::time_t now = ::time(nullptr);

	// get time info
	const ::tm* const time = ::localtime(&now);

	// check error
	if (time == nullptr)
		return;


	// -- day -----------------------------------------------------------------

	_buffer[ 1U] = static_cast<char>((time->tm_mday / 10) ^ 48);
	_buffer[ 2U] = static_cast<char>((time->tm_mday % 10) ^ 48);


	// -- month ---------------------------------------------------------------

	const auto month = time->tm_mon + 1;
	_buffer[ 4U] = static_cast<char>((month / 10) ^ 48);// + '0';
	_buffer[ 5U] = static_cast<char>((month % 10) ^ 48);// + '0';


	// -- year ----------------------------------------------------------------

	const auto year = time->tm_year + 1900;
	_buffer[ 7U] = static_cast<char> ((year / 1000) ^ 48);
	_buffer[ 8U] = static_cast<char>(((year /  100) % 10) ^ 48);
	_buffer[ 9U] = static_cast<char>(((year /   10) % 10) ^ 48);
	_buffer[10U] = static_cast<char> ((year %   10) ^ 48);


	// -- hour ----------------------------------------------------------------

	_buffer[12U] = static_cast<char>((time->tm_hour / 10) ^ 48);
	_buffer[13U] = static_cast<char>((time->tm_hour % 10) ^ 48);


	// -- minute --------------------------------------------------------------

	_buffer[15U] = static_cast<char>((time->tm_min / 10) ^ 48);
	_buffer[16U] = static_cast<char>((time->tm_min % 10) ^ 48);


	// -- second --------------------------------------------------------------

	_buffer[18U] = static_cast<char>((time->tm_sec / 10) ^ 48);
	_buffer[19U] = static_cast<char>((time->tm_sec % 10) ^ 48);
}


// -- public accessors --------------------------------------------------------

/* data */
auto sm::timestamp::data(void) const noexcept -> const char(&)[22U] {
	return _buffer;
}

/* size */
auto sm::timestamp::size(void) const noexcept -> unsigned {
	return 21U;
}
