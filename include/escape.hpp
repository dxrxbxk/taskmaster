#ifndef escape_hpp
#define escape_hpp

#include <string_view>
#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E S C A P E ---------------------------------------------------------

	class escape final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::escape;


		public:

			// -- public static methods ---------------------------------------

			/* move home */
			static consteval auto move_home(void) noexcept -> std::string_view {
				return std::string_view{"\x1b[H"};
			}

			/* enable cursor */
			static consteval auto enable_cursor(void) noexcept -> std::string_view {
				return std::string_view{"\x1b[?25h"};
			}

			/* disable cursor */
			static consteval auto disable_cursor(void) noexcept -> std::string_view {
				return std::string_view{"\x1b[?25l"};
			}

			/* move up */
			static auto move_up(const unsigned short& distance = 1U) -> std::string {
				return std::string{self::_move_direction(distance, 'A')};
			}

			/* move down */
			static auto move_down(const unsigned short& distance = 1U) -> std::string {
				return std::string{self::_move_direction(distance, 'B')};
			}

			/* move left */
			static auto move_left(const unsigned short& distance = 1U) -> std::string {
				return std::string{self::_move_direction(distance, 'D')};
			}

			/* move right */
			static auto move_right(const unsigned short& distance = 1U) -> std::string {
				return std::string{self::_move_direction(distance, 'C')};
			}


		private:

			/* move direction */
			static auto _move_direction(unsigned short cells, const char& direction) noexcept -> std::string {

				if (cells == 0U)
					return std::string{};

				// compile time buffer size
				constexpr const auto size =
					3U // escape + ctrl + direction
					+ 5U; // unsigned short max digits

				// buffer
				char buffer[size];

				// init iterator to last index
				auto i = size - 1U;

				// direction character
				buffer[i] = direction;

				// integer to ascii X pos
				while (cells) {
					buffer[--i] = static_cast<char>((cells % 10U) ^ 48U);
					cells /= 10U;
				}

				// ctrl character
				buffer[--i] = '[';
				// escape character
				buffer[--i] = '\x1b';

				// append escape sequence to buffer
				return std::string{&buffer[i], size - i};
			}



	}; // class escape

} // namespace sm

#endif // escape_hpp
