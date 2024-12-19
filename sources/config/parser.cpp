#include "config/parser.hpp"


//const char* states[] {
//	"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::parser::parser(void) noexcept 
: _it{nullptr}, _end{nullptr}, _tr{&_machine[S_START][C_EOB]},
  _count{0U}, _line{1U}, _action{nullptr}, _buffer{}, _values{}, _pm{nullptr}, _profile{} {
}


#include <iostream>
// -- private methods ---------------------------------------------------------

/* parse implementation */
auto sm::parser::_parse(void) -> void {

	// loop over bytes
	for (; _it != _end; ++_it) {

		// get next transition
		_tr = &_machine[_tr->state][_char_table[
			reinterpret_cast<const ::uint8_t&>(*_it)]];
		(this->*_tr->action)();
	}

	// trigger end of buffer
	_tr = &_machine[_tr->state][C_EOB];
	(this->*_tr->action)();
}


/* skip */
auto sm::parser::_skip(void) noexcept -> void {}

/* newline */
auto sm::parser::_newline(void) -> void {
	++_line;
}

/* increment */
auto sm::parser::_increment(void) noexcept -> void {
	++_count;
}

/* add id */
auto sm::parser::_add_id(void) -> void {

	self::_flush();

	if (_profile)
		_pm->add_profile(std::move(_profile));

	// create new program
	_profile = sm::make_shared<sm::profile>(std::move(_buffer));

	_buffer.clear();
}

/* add key */
auto sm::parser::_add_key(void) -> void {
	self::_flush();
	_action = self::_search_key(_buffer);
	_count = 0U;
	_buffer.clear();
}

/* add value */
auto sm::parser::_add_value(void) -> void {
	self::_flush();
	std::cout << std::hex << _buffer << '\r' << std::endl;
	_values.push_back(_buffer);
	(this->*_action)();
	_buffer.clear();
}

/* flush */
auto sm::parser::_flush(void) -> void {
	_buffer.append(_it - _count, _count);
	_count = 0U;
}

/* error */
auto sm::parser::_error(void) -> void {
	throw sm::runtime_error("parser error");
}
