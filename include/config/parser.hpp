#ifndef parser_hpp
#define parser_hpp

#include "reader.hpp"
#include "resources/unique_ptr.hpp"
#include "program.hpp"
#include "program_manager.hpp"
#include "utils/atoi.hpp"
#include "utils/is_dir.hpp"
#include "signal.hpp"

#include <iostream>
#include <sys/stat.h>
#include <filesystem>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P A R S E R ---------------------------------------------------------

	class parser {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::parser;

			/* action type */
			using action_type = auto (parser::*)(void) -> void;


			// -- forward declarations ----------------------------------------

			/* transition */
			struct transition;


			// -- private members ---------------------------------------------

			/* iterator */
			const char* _it;

			/* end iterator */
			const char* _end;

			/* transition */
			const transition* _tr;


			/* count */
			sm::usize _count;

			/* line */
			sm::usize _line;

			/* action */
			action_type _action;

			/* buffer */
			std::string _buffer;


			/* program manager */
			sm::program_manager* _pm;

			/* program */
			sm::unique_ptr<sm::program> _program;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			parser(void) noexcept;

			/* deleted copy constructor */
			parser(const self&) = delete;

			/* deleted move constructor */
			parser(self&&) = delete;

			/* destructor */
			~parser(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			template <unsigned int N>
			void parse(const sm::reader<N>& buffer, sm::program_manager& pm) {

				// check for end of file
				if (buffer.eof()) {

					if (_program)
						_pm->add_program(std::move(_program));

					return;
				}

				// set program manager
				_pm = &pm;

				// get iterator
				_it  = buffer.data();
				_end = buffer.data() + buffer.size();

				// parse implementation
				self::_parse();
			}


		private:


			// -- private methods ---------------------------------------------

			/* parse implementation */
			auto _parse(void) -> void;

			/* skip */
			auto _skip(void) noexcept -> void;

			/* newline */
			auto _newline(void) -> void;

			/* increment */
			auto _increment(void) noexcept -> void;

			/* add id */
			auto _add_id(void) -> void;

			/* add key */
			auto _add_key(void) -> void;

			/* add value */
			auto _add_value(void) -> void;

			/* error */
			auto _error(void) -> void;

			/* flush */
			auto _flush(void) -> void;


			// -- private constants -------------------------------------------

			enum state_type {

				S_START,

				S_DEFAULT,

				S_WAIT_NEWLINE,
				S_IN_COMMENT,

				S_WAIT_ID,
				S_IN_ID,
				S_WAIT_CLOSE_ID,


				S_IN_KEY,
				S_WAIT_ASSIGN,
				S_WAIT_VALUE,
				S_IN_VALUE,


				S_STATE_MAX,
				S_ERROR, // must be after S_STATE_MAX
			};

			enum char_type {

				C_INVALID,
				C_NEWLINE,
				C_WHITESPACE,

				C_DIGIT,
				C_ALPHA,
				C_UNDERSCORE,

				C_SIMPLE,
				C_DOUBLE,

				C_SYMBOL,

				C_HASH,
				C_EQUAL,
				C_DOLLAR,
				C_COMMA,

				C_LEFT_BRACKET,
				C_RIGHT_BRACKET,

				C_EOB,

				C_CHAR_MAX,
			};

			struct transition final {
				state_type state;
				void (parser::*action)();
			};

			static constexpr transition _machine[S_STATE_MAX][C_CHAR_MAX] {

				// -- START ---------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_START,         &parser::_newline},
					// WHITESPACE
					{S_START,         &parser::_skip},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_ERROR,         &parser::_error},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// HASH
					{S_IN_COMMENT,    &parser::_skip},
					// EQUAL
					{S_ERROR,         &parser::_error},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_WAIT_ID,       &parser::_skip},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error},
					// EOB
					{S_START,         &parser::_skip},
				},


				// -- DEFAULT -------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_DEFAULT,       &parser::_newline},
					// WHITESPACE
					{S_DEFAULT,       &parser::_skip},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_IN_KEY,        &parser::_increment},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_ERROR,         &parser::_error},
					// HASH
					{S_IN_COMMENT,    &parser::_skip},
					// EQUAL
					{S_ERROR,         &parser::_error},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_WAIT_ID,       &parser::_skip},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error},
					// EOB
					{S_DEFAULT,       &parser::_skip},
				},


				// -- WAIT NEWLINE --------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_DEFAULT,       &parser::_newline},
					// WHITESPACE
					{S_WAIT_NEWLINE,  &parser::_skip},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_ERROR,         &parser::_error},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_ERROR,         &parser::_error},
					// HASH
					{S_IN_COMMENT,    &parser::_skip},
					// EQUAL
					{S_ERROR,         &parser::_error},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error},
					// EOB
					{S_WAIT_NEWLINE,  &parser::_skip},
				},


				// -- IN COMMENT ----------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_DEFAULT,       &parser::_newline},
					// WHITESPACE
					{S_IN_COMMENT,    &parser::_skip},
					// DIGIT
					{S_IN_COMMENT,    &parser::_skip},
					// ALPHA
					{S_IN_COMMENT,    &parser::_skip},
					// SIMPLE
					{S_IN_COMMENT,    &parser::_skip},
					// DOUBLE
					{S_IN_COMMENT,    &parser::_skip},
					// SYMBOL
					{S_IN_COMMENT,    &parser::_skip},
					// UNDERSCORE
					{S_IN_COMMENT,    &parser::_skip},
					// HASH
					{S_IN_COMMENT,    &parser::_skip},
					// EQUAL
					{S_IN_COMMENT,    &parser::_skip},
					// DOLLAR
					{S_IN_COMMENT,    &parser::_skip},
					// COMMA
					{S_IN_COMMENT,    &parser::_skip},
					// LEFT_BRACKET
					{S_IN_COMMENT,    &parser::_skip},
					// RIGHT_BRACKET
					{S_IN_COMMENT,    &parser::_skip},
					// EOB
					{S_IN_COMMENT,    &parser::_skip},
				},


				// -- WAIT ID -------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_ID,       &parser::_skip},
					// DIGIT
					{S_IN_ID,         &parser::_increment},
					// ALPHA
					{S_IN_ID,         &parser::_increment},
					// SIMPLE
					{S_IN_ID,         &parser::_increment},
					// DOUBLE
					{S_IN_ID,         &parser::_increment},
					// SYMBOL
					{S_IN_ID,         &parser::_increment},
					// UNDERSCORE
					{S_IN_ID,         &parser::_increment},
					// HASH
					{S_IN_ID,         &parser::_increment},
					// EQUAL
					{S_IN_ID,         &parser::_increment},
					// DOLLAR
					{S_IN_ID,         &parser::_increment},
					// COMMA
					{S_IN_ID,         &parser::_increment},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_skip},
					// EOB
					{S_WAIT_ID,       &parser::_skip},
				},


				// -- IN ID ---------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_CLOSE_ID, &parser::_add_id},
					// DIGIT
					{S_IN_ID,         &parser::_increment},
					// ALPHA
					{S_IN_ID,         &parser::_increment},
					// SIMPLE
					{S_IN_ID,         &parser::_increment},
					// DOUBLE
					{S_IN_ID,         &parser::_increment},
					// SYMBOL
					{S_IN_ID,         &parser::_increment},
					// UNDERSCORE
					{S_IN_ID,         &parser::_increment},
					// HASH
					{S_IN_ID,         &parser::_increment},
					// EQUAL
					{S_IN_ID,         &parser::_increment},
					// DOLLAR
					{S_IN_ID,         &parser::_increment},
					// COMMA
					{S_IN_ID,         &parser::_increment},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_WAIT_NEWLINE,  &parser::_add_id},
					// EOB
					{S_IN_ID,         &parser::_flush},
				},


				// -- WAIT CLOSE ID -------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_CLOSE_ID, &parser::_skip},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_ERROR,         &parser::_error},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_ERROR,         &parser::_error},
					// HASH
					{S_ERROR,         &parser::_error},
					// EQUAL
					{S_ERROR,         &parser::_error},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_WAIT_NEWLINE,  &parser::_skip},
					// EOB
					{S_WAIT_CLOSE_ID, &parser::_skip},
				},


				// -- IN KEY --------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_ASSIGN,   &parser::_add_key},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_IN_KEY,        &parser::_increment},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_IN_KEY,        &parser::_increment},
					// HASH
					{S_ERROR,         &parser::_error},
					// EQUAL
					{S_WAIT_VALUE,    &parser::_add_key},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error},
					// EOB
					{S_IN_KEY,        &parser::_flush},
				},

				// -- WAIT ASSIGN ---------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_ASSIGN,   &parser::_skip},
					// DIGIT
					{S_ERROR,         &parser::_error},
					// ALPHA
					{S_ERROR,         &parser::_error},
					// SIMPLE
					{S_ERROR,         &parser::_error},
					// DOUBLE
					{S_ERROR,         &parser::_error},
					// SYMBOL
					{S_ERROR,         &parser::_error},
					// UNDERSCORE
					{S_ERROR,         &parser::_error},
					// HASH
					{S_ERROR,         &parser::_error},
					// EQUAL
					{S_WAIT_VALUE,    &parser::_skip},
					// DOLLAR
					{S_ERROR,         &parser::_error},
					// COMMA
					{S_ERROR,         &parser::_error},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error},
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error},
					// EOB
					{S_WAIT_ASSIGN,   &parser::_skip},
				},


				// -- WAIT VALUE ----------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_ERROR,         &parser::_error},
					// WHITESPACE
					{S_WAIT_VALUE,    &parser::_skip},
					// DIGIT
					{S_IN_VALUE,      &parser::_increment},
					// ALPHA
					{S_IN_VALUE,      &parser::_increment},
					// SIMPLE
					{S_ERROR,         &parser::_error}, // see later
					// DOUBLE
					{S_ERROR,         &parser::_error}, // see later
					// SYMBOL
					{S_IN_VALUE,      &parser::_increment},
					// UNDERSCORE
					{S_IN_VALUE,      &parser::_increment},
					// HASH
					{S_IN_VALUE,      &parser::_increment},
					// EQUAL
					{S_IN_VALUE,      &parser::_increment},
					// DOLLAR
					{S_ERROR,         &parser::_error}, // see later
					// COMMA
					{S_IN_VALUE,      &parser::_increment},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error}, // see later
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error}, // see later
					// EOB
					{S_WAIT_VALUE,    &parser::_skip},
				},


				// -- IN VALUE ------------------------------------------------

				{
					// INVALID
					{S_ERROR,         &parser::_error},
					// NEWLINE
					{S_DEFAULT,       &parser::_add_value}, // need to add newline
					// WHITESPACE
					{S_WAIT_NEWLINE,  &parser::_add_value},
					// DIGIT
					{S_IN_VALUE,      &parser::_increment},
					// ALPHA
					{S_IN_VALUE,      &parser::_increment},
					// SIMPLE
					{S_ERROR,         &parser::_error}, // see later
					// DOUBLE
					{S_ERROR,         &parser::_error}, // see later
					// SYMBOL
					{S_IN_VALUE,      &parser::_increment},
					// UNDERSCORE
					{S_IN_VALUE,      &parser::_increment},
					// HASH
					{S_IN_VALUE,      &parser::_increment},
					// EQUAL
					{S_IN_VALUE,      &parser::_increment},
					// DOLLAR
					{S_ERROR,         &parser::_error}, // see later
					// COMMA
					{S_IN_VALUE,      &parser::_increment},
					// LEFT_BRACKET
					{S_ERROR,         &parser::_error}, // see later
					// RIGHT_BRACKET
					{S_ERROR,         &parser::_error}, // see later
					// EOB
					{S_IN_VALUE,      &parser::_flush},
				},

			};


			static constexpr char_type _char_table[256U] {

				// 0x00 - 0x08 (control characters)
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID,

				// 0x09 (horizontal tab)
				C_WHITESPACE,

				// 0x0A - 0x0C (newline, vertical tab, form feed)
				C_NEWLINE, C_NEWLINE, C_NEWLINE,

				// 0x0D (return)
				C_INVALID,

				// 0x0E - 0x1F (control characters)
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID,

				// 0x20 (space)
				C_WHITESPACE,

				// 0x21 (exclamation mark)
				C_SYMBOL,

				// 0x22 (double quote)
				C_DOUBLE,

				// 0x23 (hash)
				C_HASH,

				// 0x24 (dollar)
				C_DOLLAR,

				// 0x25 - 0x26 (percent, ampersand)
				C_SYMBOL, C_SYMBOL,

				// 0x27 (single quote)
				C_SIMPLE,

				// 0x28 - 0x2B (left parenthesis, right parenthesis, asterisk, plus)
				C_SYMBOL, C_SYMBOL, C_SYMBOL, C_SYMBOL,

				// 0x2C (comma)
				C_COMMA,

				// 0x2D - 0x2F (minus, dot, slash)
				C_SYMBOL, C_SYMBOL, C_SYMBOL,

				// 0x30 - 0x39 (0-9)
				C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
				C_DIGIT, C_DIGIT, C_DIGIT, C_DIGIT,
				C_DIGIT, C_DIGIT,

				// 0x3A - 0x3C (colon, semicolon, less)
				C_SYMBOL, C_SYMBOL, C_SYMBOL,

				// 0x3D (equal)
				C_EQUAL,

				// 0x3E - 0x40 (greater, question, at)
				C_SYMBOL, C_SYMBOL, C_SYMBOL,

				// 0x41 - 0x5A (A-Z)
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA,

				// 0x5B (left bracket)
				C_LEFT_BRACKET,

				// 0x5C (backslash)
				C_SYMBOL,

				// 0x5D (right bracket)
				C_RIGHT_BRACKET,

				// 0x5E (caret)
				C_SYMBOL,

				// 0x5F (underscore)
				C_UNDERSCORE,

				// 0x60 (grave)
				C_SYMBOL,

				// 0x61 - 0x7A (a-z)
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA, C_ALPHA, C_ALPHA,
				C_ALPHA, C_ALPHA,

				// 0x7B - 0x7E (left brace, vertical bar, right brace, tilde)
				C_SYMBOL, C_SYMBOL, C_SYMBOL, C_SYMBOL,

				// 0x7F - 0xFF (invalid characters)
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID, C_INVALID, C_INVALID, C_INVALID,
				C_INVALID,

			}; // _char_table





			auto _cmd(void) -> void {

				if (sm::is_command(_buffer.data()) == false)
					throw sm::runtime_error("invalid command");

				_program->cmd_push(std::move(_buffer.data()));
			}

			auto _numprocs(void) -> void {

				auto numprocs = sm::atoi<unsigned>(_buffer.data());

				if (numprocs == 0U)
					throw sm::runtime_error("invalid numprocs");

				_program->numprocs(numprocs);
			}

			auto _umask(void) -> void {


				// check if value is in valid range
				auto umask = sm::atoi<::mode_t, sm::oct>(_buffer.data());

				if ((umask & 0777) != umask)
					throw sm::runtime_error("invalid umask");

				_program->umask(umask);

			}

			auto _workingdir(void) -> void {

				if (sm::is_dir(_buffer.data()) == false)
					throw sm::runtime_error("invalid working directory");

				_program->workingdir(std::move(_buffer));
			}

			auto _autostart(void) -> void {
				_program->autostart(sm::atoi<bool>(_buffer.data()));
			}

			auto _autorestart(void) -> void {
				_program->autorestart(sm::atoi<bool>(_buffer.data()));
			}

			auto _exitcodes(void) -> void {
				_program->exitcodes(sm::atoi<int>(_buffer.data()));
			}

			auto _startretries(void) -> void {
				_program->startretries(sm::atoi<unsigned>(_buffer.data()));
			}


			auto _starttime(void) -> void {
				_program->starttime(sm::atoi<unsigned>(_buffer.data()));
			}

			auto _stopsignal(void) -> void {

				_program->stopsignal(sm::signal::to_int(_buffer.data()));
			}

			auto _stoptime(void) -> void {
				_program->stoptime(sm::atoi<unsigned>(_buffer.data()));
			}

			auto _stdout(void) -> void {
				std::filesystem::path parent = std::filesystem::path{_buffer.data()}.parent_path();

				if (is_dir(parent.c_str()) == false)
					throw sm::runtime_error("invalid stdout path");

				_program->stdout(std::move(_buffer));

			}

			auto _stderr(void) -> void {

				std::filesystem::path parent = std::filesystem::path{_buffer.data()}.parent_path();
				if (is_dir(parent.c_str()) == false)
					throw sm::runtime_error("invalid stderr path");
				_program->stderr(std::move(_buffer));
			}

			auto _env(void) -> void {
				_program->env_push(_buffer.data());
			}


			static constexpr action_type _actions[] {
				&parser::_cmd,
				&parser::_numprocs,
				&parser::_umask,
				&parser::_workingdir,
				&parser::_autostart,
				&parser::_autorestart,
				&parser::_exitcodes,
				&parser::_startretries,
				&parser::_starttime,
				&parser::_stopsignal,
				&parser::_stoptime,
				&parser::_stdout,
				&parser::_stderr,
				&parser::_env,
			};

			static constexpr std::string_view _keywords[] {
				"cmd",
				"numprocs",
				"umask",
				"workingdir",
				"autostart",
				"autorestart",
				"exitcodes",
				"startretries",
				"starttime",
				"stopsignal",
				"stoptime",
				"stdout",
				"stderr",
				"env",
			};


			static auto _search_key(const std::string_view& key) -> action_type {

				constexpr auto size = sizeof(_keywords)
									/ sizeof(_keywords[0U]);

				// loop over keywords
				for (sm::usize i = 0U; i < size; ++i) {

					if (key == _keywords[i])
						return _actions[i];
				}

				std::cout << "unknown keyword: " << key << std::endl;

				// keyword not found
				throw sm::runtime_error("unknown keyword");
			}



	}; // class parser


	/* tester */
	auto parser_tester(void) -> void;





} // namespace sm

#endif // parser_hpp
