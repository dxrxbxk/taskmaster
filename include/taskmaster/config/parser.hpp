#ifndef PARSER_HPP
# define PARSER_HPP

#include "common/reader.hpp"

namespace sm {

class parser {
	public:
		template <unsigned int N>
		void parse(const sm::reader<N> &buf) {
			_it = buf.data();
			_end = buf.data() + buf.size();
			_parse();
		}

	private:
		struct transition;

		const uint8_t *_it;
		const uint8_t *_end;
		const transition *_tr;

		void _parse();

		void _skip();
		void _increment();
		void _add_title();
		void _add_key();
		void _add_value();
		void _error();

		enum state_type {
			S_START,
			S_DEFAULT,

			S_WAIT_TITLE,
			S_IN_TITLE,
			S_END_TITLE,

			S_IN_KEY,
			S_END_KEY,

			S_WAIT_VALUE,

			S_START_TABLE,
			S_IN_TALBE,
			S_END_TABLE,

			S_IN_VALUE,
			S_END_VALUE,

			S_IN_STRING,
			S_END_STRING,

			S_EXPAND,

			S_ERROR,

			S_SIZE,
		};

		enum char_type {
			C_PRINTABLE,
			C_WHITESPACE,
			C_COMMENT,
			C_NEWLINE,
			C_RETURN,
			C_CONTROL,
			C_EQUAL,
			C_LBRACKET,
			C_RBRACKET,
			C_QUOTE,
			C_DQUOTE,
			C_DOLLAR,
			C_COMMA,
			C_EOB,
			C_SIZE,
		};

		struct transition {
			state_type state;
			void (parser::*action)();
		};

		static constexpr transition _machine[S_SIZE][C_SIZE] = {
			/* START */
			{
				{S_START, &parser::_skip}, /* C_PRINTABLE */
				{S_START, &parser::_skip}, /* C_WHITESPACE */
				{S_START, &parser::_skip}, /* C_COMMENT */
				{S_START, &parser::_skip}, /* C_NEWLINE */
				{S_START, &parser::_skip}, /* C_RETURN */
				{S_START, &parser::_skip}, /* C_CONTROL */
				{S_START, &parser::_skip}, /* C_EQUAL */
				{S_WAIT_TITLE, &parser::_skip}, /* C_LBRACKET */
				{S_START, &parser::_skip}, /* C_RBRACKET */
				{S_START, &parser::_skip}, /* C_QUOTE */
				{S_START, &parser::_skip}, /* C_DQUOTE */
				{S_START, &parser::_skip}, /* C_DOLLAR */
				{S_START, &parser::_skip}, /* C_COMMA */
				{S_START, &parser::_skip}, /* C_EOB */
			},
			/* DEFAULT */
			{
				{S_IN_KEY, &parser::_skip}, /* C_PRINTABLE */
				{S_DEFAULT, &parser::_skip}, /* C_WHITESPACE */
				{S_DEFAULT, &parser::_skip}, /* C_COMMENT */
				{S_DEFAULT, &parser::_skip}, /* C_NEWLINE */
				{S_DEFAULT, &parser::_skip}, /* C_RETURN */
				{S_DEFAULT, &parser::_skip}, /* C_CONTROL */
				{S_DEFAULT, &parser::_skip}, /* C_EQUAL */
				{S_WAIT_TITLE, &parser::_skip}, /* C_LBRACKET */
				{S_DEFAULT, &parser::_skip}, /* C_RBRACKET */
				{S_DEFAULT, &parser::_skip}, /* C_QUOTE */
				{S_DEFAULT, &parser::_skip}, /* C_DQUOTE */
				{S_DEFAULT, &parser::_skip}, /* C_DOLLAR */
				{S_DEFAULT, &parser::_skip}, /* C_COMMA */
				{S_DEFAULT, &parser::_skip}, /* C_EOB */
			},

			/* WAIT_TITLE */
			{
				{S_IN_TITLE, &parser::_skip}, /* C_PRINTABLE */
				{S_WAIT_TITLE, &parser::_skip}, /* C_WHITESPACE */
				{S_WAIT_TITLE, &parser::_skip}, /* C_COMMENT */
				{S_WAIT_TITLE, &parser::_skip}, /* C_NEWLINE */
				{S_WAIT_TITLE, &parser::_skip}, /* C_RETURN */
				{S_WAIT_TITLE, &parser::_skip}, /* C_CONTROL */
				{S_WAIT_TITLE, &parser::_skip}, /* C_EQUAL */
				{S_WAIT_TITLE, &parser::_skip}, /* C_LBRACKET */
				{S_WAIT_TITLE, &parser::_skip}, /* C_RBRACKET */
				{S_WAIT_TITLE, &parser::_skip}, /* C_QUOTE */
				{S_WAIT_TITLE, &parser::_skip}, /* C_DQUOTE */
				{S_WAIT_TITLE, &parser::_skip}, /* C_DOLLAR */
				{S_WAIT_TITLE, &parser::_skip}, /* C_COMMA */
				{S_WAIT_TITLE, &parser::_skip}, /* C_EOB */
			},

			/* S_IN_TITLE */
			{
				{S_IN_TITLE, &parser::_increment}, /* C_PRINTABLE */
				{S_END_TITLE, &parser::_add_title}, /* C_WHITESPACE */
				{S_IN_TITLE, &parser::_skip}, /* C_COMMENT */
				{S_IN_TITLE, &parser::_skip}, /* C_NEWLINE */
				{S_IN_TITLE, &parser::_skip}, /* C_RETURN */
				{S_IN_TITLE, &parser::_skip}, /* C_CONTROL */
				{S_IN_TITLE, &parser::_skip}, /* C_EQUAL */
				{S_IN_TITLE, &parser::_skip}, /* C_LBRACKET */
				{S_IN_TITLE, &parser::_skip}, /* C_RBRACKET */
				{S_IN_TITLE, &parser::_skip}, /* C_QUOTE */
				{S_IN_TITLE, &parser::_skip}, /* C_DQUOTE */
				{S_IN_TITLE, &parser::_skip}, /* C_DOLLAR */
				{S_IN_TITLE, &parser::_skip}, /* C_COMMA */
				{S_IN_TITLE, &parser::_skip}, /* C_EOB */
			},

			/* S_END_TITLE */
			{ 
				{S_END_TITLE, &parser::_skip}, /* C_PRINTABLE */
				{S_END_TITLE, &parser::_skip}, /* C_WHITESPACE */
				{S_END_TITLE, &parser::_skip}, /* C_COMMENT */
				{S_END_TITLE, &parser::_skip}, /* C_NEWLINE */
				{S_END_TITLE, &parser::_skip}, /* C_RETURN */
				{S_END_TITLE, &parser::_skip}, /* C_CONTROL */
				{S_END_TITLE, &parser::_skip}, /* C_EQUAL */
				{S_END_TITLE, &parser::_skip}, /* C_LBRACKET */
				{S_DEFAULT, &parser::_skip}, /* C_RBRACKET */
				{S_END_TITLE, &parser::_skip}, /* C_QUOTE */
				{S_END_TITLE, &parser::_skip}, /* C_DQUOTE */
				{S_END_TITLE, &parser::_skip}, /* C_DOLLAR */
				{S_END_TITLE, &parser::_skip}, /* C_COMMA */
				{S_END_TITLE, &parser::_skip}, /* C_EOB */
			},
			/* S_IN_KEY */
			{
				{S_IN_KEY, &parser::_increment}, /* C_PRINTABLE */
				{S_END_KEY, &parser::_skip}, /* C_WHITESPACE */
				{S_IN_KEY, &parser::_skip}, /* C_COMMENT */
				{S_IN_KEY, &parser::_skip}, /* C_NEWLINE */
				{S_IN_KEY, &parser::_skip}, /* C_RETURN */
				{S_IN_KEY, &parser::_skip}, /* C_CONTROL */
				{S_IN_KEY, &parser::_skip}, /* C_EQUAL */
				{S_IN_KEY, &parser::_skip}, /* C_LBRACKET */
				{S_IN_KEY, &parser::_skip}, /* C_RBRACKET */
				{S_IN_KEY, &parser::_skip}, /* C_QUOTE */
				{S_IN_KEY, &parser::_skip}, /* C_DQUOTE */
				{S_IN_KEY, &parser::_skip}, /* C_DOLLAR */
				{S_IN_KEY, &parser::_skip}, /* C_COMMA */
				{S_IN_KEY, &parser::_skip}, /* C_EOB */
			},

			/* S_END_KEY */
			{
				{S_END_KEY, &parser::_skip}, /* C_PRINTABLE */
				{S_END_KEY, &parser::_skip}, /* C_WHITESPACE */
				{S_END_KEY, &parser::_skip}, /* C_COMMENT */
				{S_END_KEY, &parser::_skip}, /* C_NEWLINE */
				{S_END_KEY, &parser::_skip}, /* C_RETURN */
				{S_END_KEY, &parser::_skip}, /* C_CONTROL */
				{S_WAIT_VALUE, &parser::_skip}, /* C_EQUAL */
				{S_END_KEY, &parser::_skip}, /* C_LBRACKET */
				{S_END_KEY, &parser::_skip}, /* C_RBRACKET */
				{S_END_KEY, &parser::_skip}, /* C_QUOTE */
				{S_END_KEY, &parser::_skip}, /* C_DQUOTE */
				{S_END_KEY, &parser::_skip}, /* C_DOLLAR */
				{S_END_KEY, &parser::_skip}, /* C_COMMA */
				{S_END_KEY, &parser::_skip}, /* C_EOB */
			},

			/* S_WAIT_VALUE */
			{
				{S_IN_VALUE, &parser::_skip}, /* C_PRINTABLE */
				{S_WAIT_VALUE, &parser::_skip}, /* C_WHITESPACE */
				{S_WAIT_VALUE, &parser::_skip}, /* C_COMMENT */
				{S_WAIT_VALUE, &parser::_skip}, /* C_NEWLINE */
				{S_WAIT_VALUE, &parser::_skip}, /* C_RETURN */
				{S_WAIT_VALUE, &parser::_skip}, /* C_CONTROL */
				{S_WAIT_VALUE, &parser::_skip}, /* C_EQUAL */
				{S_START_TABLE, &parser::_skip}, /* C_LBRACKET */
				{S_WAIT_VALUE, &parser::_skip}, /* C_RBRACKET */
				{S_WAIT_VALUE, &parser::_skip}, /* C_QUOTE */
				{S_WAIT_VALUE, &parser::_skip}, /* C_DQUOTE */
				{S_WAIT_VALUE, &parser::_skip}, /* C_DOLLAR */
				{S_WAIT_VALUE, &parser::_skip}, /* C_COMMA */
				{S_WAIT_VALUE, &parser::_skip}, /* C_EOB */
			},

			/* S_START_TABLE */
			{
				{S_IN_TALBE, &parser::_skip}, /* C_PRINTABLE */
				{S_START_TABLE, &parser::_skip}, /* C_WHITESPACE */
				{S_START_TABLE, &parser::_skip}, /* C_COMMENT */
				{S_START_TABLE, &parser::_skip}, /* C_NEWLINE */
				{S_START_TABLE, &parser::_skip}, /* C_RETURN */
				{S_START_TABLE, &parser::_skip}, /* C_CONTROL */
				{S_START_TABLE, &parser::_skip}, /* C_EQUAL */
				{S_START_TABLE, &parser::_skip}, /* C_LBRACKET */
				{S_START_TABLE, &parser::_skip}, /* C_RBRACKET */
				{S_START_TABLE, &parser::_skip}, /* C_QUOTE */
				{S_START_TABLE, &parser::_skip}, /* C_DQUOTE */
				{S_START_TABLE, &parser::_skip}, /* C_DOLLAR */
				{S_START_TABLE, &parser::_skip}, /* C_COMMA */
				{S_START_TABLE, &parser::_skip}, /* C_EOB */
			},

			/* S_IN_TABLE */
			{
				{S_IN_TALBE, &parser::_add_value}, /* C_PRINTABLE */
				{S_IN_TALBE, &parser::_skip}, /* C_WHITESPACE */
				{S_IN_TALBE, &parser::_skip}, /* C_COMMENT */
				{S_IN_TALBE, &parser::_skip}, /* C_NEWLINE */
				{S_IN_TALBE, &parser::_skip}, /* C_RETURN */
				{S_IN_TALBE, &parser::_skip}, /* C_CONTROL */
				{S_IN_TALBE, &parser::_skip}, /* C_EQUAL */
				{S_IN_TALBE, &parser::_skip}, /* C_LBRACKET */
				{S_IN_TALBE, &parser::_skip}, /* C_RBRACKET */
				{S_IN_TALBE, &parser::_skip}, /* C_QUOTE */
				{S_IN_TALBE, &parser::_skip}, /* C_DQUOTE */
				{S_IN_TALBE, &parser::_skip}, /* C_DOLLAR */
				{S_IN_TALBE, &parser::_skip}, /* C_COMMA */
				{S_IN_TALBE, &parser::_skip}, /* C_EOB */
			},
				
		};


		static constexpr char_type _char_table[256] = {
			// ASCII 0-31: Contrôle (non imprimable)
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_WHITESPACE, C_NEWLINE, C_CONTROL, C_CONTROL, C_RETURN, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,

			// ASCII 32-47: Espaces et symboles
			C_WHITESPACE, C_PRINTABLE, C_QUOTE, C_PRINTABLE, C_DOLLAR, C_PRINTABLE, C_PRINTABLE, C_QUOTE,
			C_LBRACKET, C_RBRACKET, C_PRINTABLE, C_PRINTABLE, C_COMMA, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,

			// ASCII 48-57: Chiffres
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE,

			// ASCII 58-64: Symboles
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_EQUAL, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,

			// ASCII 65-90: Lettres majuscules
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,

			// ASCII 91-96: Symboles
			C_LBRACKET, C_PRINTABLE, C_RBRACKET, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,

			// ASCII 97-122: Lettres minuscules
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE,

			// ASCII 123-127: Symboles et DEL
			C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_PRINTABLE, C_CONTROL,

			// ASCII 128-255: Étendu (non imprimable ou définissable au besoin)
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL,
			C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL, C_CONTROL
		};



	public:

		parser() noexcept;
		~parser() = default;
		parser(const parser &src) = default;
		parser &operator=(const parser &src) = default;
		parser(parser &&src) = default;
		parser &operator=(parser &&src) = default;

};

}

#endif
