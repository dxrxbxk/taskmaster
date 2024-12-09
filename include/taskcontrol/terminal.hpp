#ifndef terminal_hpp
#define terminal_hpp

#include <vector>
#include <termios.h>
#include <sys/ioctl.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {



	// -- T E R M I N A L -----------------------------------------------------

	class terminal final {


		public:

			// -- public classes ----------------------------------------------

			/* observer */
			class observer {


				public:

					// -- public lifecycle ------------------------------------

					/* default constructor */
					observer(void) {
						terminal::subscribe(*this);
					}

					/* copy constructor */
					observer(const observer&) noexcept = default;

					/* move constructor */
					observer(observer&&) noexcept = default;

					/* destructor */
					virtual ~observer(void) noexcept {
						terminal::unsubscribe(*this);
					}


					// -- public assignment operators -------------------------

					/* copy assignment operator */
					auto operator=(const observer&) noexcept -> observer& = default;

					/* move assignment operator */
					auto operator=(observer&&) noexcept -> observer& = default;


					// -- public interface ------------------------------------

					/* on resize */
					virtual auto on_resize(const unsigned short&,
										   const unsigned short&) noexcept -> void = 0;

			}; // class observer


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::terminal;


			// -- private members ---------------------------------------------

			/* terminal attributes */
			const struct ::termios _origin;
				  struct ::termios _raw;

			/* terminal size */
			struct ::winsize _size;

			/* observers */
			std::vector<self::observer*> _observers;



			// -- private static methods --------------------------------------

			/* get */
			static auto _get(void) -> struct ::termios;

			/* set */
			static auto _set(const struct ::termios&) -> void;

			/* setup raw mode */
			static auto _setup_raw(struct ::termios&) noexcept -> void;

			/* shared */
			static auto _shared(void) -> self&;

			/* resize handler */
			static auto _resize_handler(int) noexcept -> void;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			terminal(void);

			/* deleted copy constructor */
			terminal(const self&) = delete;

			/* deleted move constructor */
			terminal(self&&) = delete;

			/* destructor */
			~terminal(void) noexcept;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


		public:

			// -- public static methods ---------------------------------------

			/* raw */
			static auto raw(void) -> void;

			/* restore */
			static auto restore(void) -> void;

			/* subscribe */
			static auto subscribe(sm::terminal::observer&) -> void;

			/* unsubscribe */
			static auto unsubscribe(sm::terminal::observer&) noexcept -> void;

			/* width */
			static auto width(void) noexcept -> unsigned short;

			/* height */
			static auto height(void) noexcept -> unsigned short;

	}; // class terminal

}

#include <iostream>
#include <unistd.h>
#include "common/diagnostics/exception.hpp"
namespace sm {

	template <bool raw>
		auto termios_test(void) -> void {

			if constexpr (raw == true) {
				sm::terminal::raw();
			}
			else {
				sm::terminal::restore();
			}


			std::cout << "\x1b[33mpress 'q' to quit\r\n\x1b[0m";

			while (true) {

				char buffer[32U];

				const auto readed = ::read(STDIN_FILENO, buffer, sizeof(buffer));

				if (readed == -1)
					throw sm::system_error("read");

				for (::ssize_t i = 0; i < readed; ++i) {

					if (buffer[i] == 'q')
						return;

					if (std::isprint(buffer[i]))
						std::cout << "\x1b[32m" << buffer[i] << "\x1b[0m";
					else
						std::cout << "\x1b[31m'\\x" << std::hex << static_cast<int>(buffer[i])
							<< std::dec << "'\x1b[0m ";
				}

				std::cout << std::flush;
			}
		}

} // namespace sm

#endif // terminal_hpp
