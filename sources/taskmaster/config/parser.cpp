#include "taskmaster/config/parser.hpp"

sm::parser::parser(void) noexcept 
: _it{nullptr}, _end{nullptr}, _tr{_machine[S_DEFAULT]} {
};

void sm::parser::_parse(void) {
	for(; _it != _end; ++_it) {
		_tr = &_machine[_tr->state][_char_table[*_it]];
		(this->*_tr->action)();
	}
	_tr = &_machine[_tr->state][C_EOB];
	(this->*_tr->action)();
};
