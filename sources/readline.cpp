#include "readline.hpp"
#include "taskmaster.hpp"
#include "string/split.hpp"


/* on event */
auto sm::readline::on_event(const sm::event& events, sm::taskmaster& tm) -> void {

	if (events.is_err())
		throw sm::runtime_error("readline: error event");

	if (not events.is_in())
		throw sm::runtime_error("readline: not an input event");

	// read input
	sm::reader<128U> reader{};
	reader.read(STDIN_FILENO);


	if (reader.size() == 1U) {

		if (reader[0U] == 'q') {
			::write(STDOUT_FILENO, "\r\x1b[2K", 5U);
			tm.stop();
			return;
		}

		// delete
		else if (reader[0U] == 127) {
			self::_delete();
			return;
		}

		// return
		else if (reader[0U] == '\r') {
			self::_return(tm);
			return;
		}

		// insert character
		else {
			self::_insert(reader[0U]);
			return;
		}
	}


	else if (reader.size() == 3U) {

		if (reader[0U] == '\x1b' && reader[1U] == '[') {

			// move cursor right
			if (reader[2U] == 'C')
				self::_move_right();

			// move cursor left
			else if (reader[2U] == 'D')
				self::_move_left();

		}
	}

}


/* return */
auto sm::readline::_return(sm::taskmaster& tm) -> void {

	::write(STDOUT_FILENO, "\r\n", 2U);

	if (_input.empty())
		return;

	// split line by space
	auto argv = sm::split(_input, " \t\v\f");

	// execute command
	tm.executor().execute(tm, std::move(argv));
	//sm::command_factory::execute(tm, std::move(argv));


	_input.clear();
	_cursor_pos = 0U;
	_input_pos = 0U;
	_offset = 0U;
}
