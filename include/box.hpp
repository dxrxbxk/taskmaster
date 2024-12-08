#ifndef box_hpp
#define box_hpp

#include "escape.hpp"
#include <string>

namespace sm {
	class box final {


		public:

			// -- public types ------------------------------------------------

			/* size type */
			using size_type = unsigned short;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::box;


			// -- private members ---------------------------------------------

			/* box */
			std::string _box;


			// -- private constants -------------------------------------------

			/* symbol enum */
			enum : unsigned {
				LINE_H, LINE_V,
				CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR,
				BORDER_MAX
			};

			/* symbols */
			static constexpr const char* _symbols[BORDER_MAX] = {
				"\xe2\x94\x80",
				"\xe2\x94\x82",
				"\xe2\x95\xad",
				"\xe2\x95\xae",
				"\xe2\x95\xb0",
				"\xe2\x95\xaf"
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			box(void) noexcept(noexcept(std::string{})) = default;

			/* copy constructor */
			box(const self&) = default;

			/* move constructor */
			box(self&&) noexcept = default;

			/* destructor */
			~box(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* set */
			auto set(const unsigned short& x,
					 const unsigned short& y,
					 const unsigned short& w,
					 const unsigned short& h) -> void {

				// avoid namespace pollution
				using esc = sm::escape;

				if (w == 0U || h == 0U)
					return;

				// clear previous box
				_box.clear();

				// append top left corner
				_box.append(_symbols[CORNER_TL], 3U);

				// append top box
				for (size_type i = 1U; i < w - 1U; ++i)
					_box.append(_symbols[LINE_H], 3U);

				// append top right corner
				_box.append(_symbols[CORNER_TR], 3U);

				// append move position bottom left corner
				_box.append(esc::move_position(x, y + h - 1));

				// append bottom left corner
				_box.append(_symbols[CORNER_BL], 3U);

				// append bottom box
				for (size_type i = 1U; i < w - 1U; ++i)
					_box.append(_symbols[LINE_H], 3U);

				// append bottom right corner
				_box.append(_symbols[CORNER_BR], 3U);


				size_type y_pos = y + h - 1U;

				// append left vertical box
				for (size_type i = y + 1U; i < y_pos; ++i) {
					// append move position left box
					_box.append(esc::move_position(x, i));
					// append left vertical box
					_box.append(_symbols[LINE_V], 3U);
				}

				size_type x_pos = x + w - 1U;

				// append right vertical box
				for (size_type i = y + 1U; i < y_pos; ++i) {
					// append move position right box
					_box.append(esc::move_position(x_pos, i));
					// append right vertical box
					_box.append(_symbols[LINE_V], 3U);
				}

			}

	}; // class box
} // namespace sm

#endif // box_hpp
