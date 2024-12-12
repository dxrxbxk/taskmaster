#include "taskmaster/config/parser.hpp"
#include "taskmaster/program.hpp"


//const char* states[] {
//	"


// -- P A R S E R -------------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::parser::parser(void) noexcept 
: _it{nullptr}, _end{nullptr}, _tr{&_machine[S_START][C_EOB]}, _back{_tr},
  _count{0U}, _action{nullptr}, _buffer{} {
};


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
};


auto sm::parser::_skip(void) noexcept -> void {};

auto sm::parser::_increment(void) noexcept -> void {
	++_count;
};

auto sm::parser::_add_id(void) -> void {
	self::_flush();
	_buffer.clear();
};

auto sm::parser::_add_key(void) -> void {
	self::_flush();
	_action = self::_search_key(_buffer);
	_count = 0U;
	_buffer.clear();
};

auto sm::parser::_add_value(void) -> void {
	self::_flush();
	(this->*_action)();
	_buffer.clear();
};

auto sm::parser::_flush(void) -> void {
	_buffer.append(_it - _count, _count);
	_count = 0U;
};


auto sm::parser::_error(void) -> void {
	throw sm::runtime_error("parser error");
};


#include "common/resources/unique_fd.hpp"
#include "common/reader.hpp"
#include "common/system/open.hpp"




auto sm::parser_tester(void) -> void {

	sm::unique_fd file = sm::open("./tools/taskmaster.conf", O_RDONLY);

	sm::reader<32U> reader;
	sm::parser parser;

	sm::program_manager pm;

	do {

		reader.read(file);
		parser.parse(reader, pm);

	} while (reader.size() > 0);
}
