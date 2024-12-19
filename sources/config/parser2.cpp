//#include "config/parser2.hpp"
//#include "program.hpp"
//#include <iostream>
//
//// -- P A R S E R -------------------------------------------------------------
//
//// -- public lifecycle --------------------------------------------------------
//
///* default constructor */
//sm::parser2::parser2(void) noexcept 
//: _it{nullptr}, _end{nullptr}, _tr{&_machine[S_START][C_EOB]},
//  _count{0U}, _line{1U}, _action{nullptr}, _buffer{}, _pm{nullptr}, _program{} {
//}
//
//
//// -- private methods ---------------------------------------------------------
//
///* parse implementation */
//auto sm::parser2::_parse(void) -> void {
//
//	// loop over bytes
//	for (; _it != _end; ++_it) {
//
//		// get character type
//		const auto type = _ctable[static_cast<sm::u8>(*_it)];
//
//		// get next transition
//		_tr = &_machine[_tr->state][type];
//
//		// execute action
//		(this->*_tr->action)();
//	}
//
//	// trigger end of buffer
//	_tr = &_machine[_tr->state][C_EOB];
//	(this->*_tr->action)();
//}
//
//
///* skip */
//auto sm::parser2::_skip(void) noexcept -> void {}
//
///* newline */
//auto sm::parser2::_newline(void) -> void {
//	++_line;
//}
//
///* increment */
//auto sm::parser2::_increment(void) noexcept -> void {
//	++_count;
//}
//
///* add id */
//auto sm::parser2::_add_id(void) -> void {
//
//	self::_flush();
//
//	if (_program)
//		_pm->add_program(std::move(_program));
//
//	// create new program
//	_program = sm::make_unique<sm::program>(std::move(_buffer));
//
//	_buffer.clear();
//}
//
///* add key */
//auto sm::parser2::_add_key(void) -> void {
//	self::_flush();
//	_action = self::_search_key(_buffer);
//	_count = 0U;
//	_buffer.clear();
//}
//
///* add value */
//auto sm::parser2::_add_value(void) -> void {
//	self::_flush();
//	(this->*_action)();
//	_buffer.clear();
//}
//
///* flush */
//auto sm::parser2::_flush(void) -> void {
//	_buffer.append(_it - _count, _count);
//	_count = 0U;
//}
//
///* error */
//auto sm::parser2::_error(void) -> void {
//	throw sm::runtime_error("parser error");
//}
//
//
//#include "resources/unique_fd.hpp"
//#include "reader.hpp"
//#include "system/open.hpp"
//
//
//auto sm::parser_tester2(void) -> void {
//
//	sm::unique_fd file = sm::open("./tools/taskmaster.conf", O_RDONLY);
//
//	sm::reader<32U> reader;
//	sm::parser2 parser;
//
//	sm::program_manager pm;
//
//	do {
//
//		reader.read(file);
//		parser.parse(reader, pm);
//
//	} while (reader.size() > 0);
//
//
//	std::cout << "number of programs: " << pm.size() << std::endl;
//}
