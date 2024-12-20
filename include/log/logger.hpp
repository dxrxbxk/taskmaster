#ifndef logger_hpp
#define logger_hpp

#include "log/timestamp.hpp"
#include "diagnostics/exception.hpp"
#include "stream.hpp"
#include "string/strlen.hpp"

#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- L O G G E R ---------------------------------------------------------

	class logger final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::logger;


			// -- private members ---------------------------------------------

			/* stream */
			sm::stream _stream;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			logger(void) noexcept = default;

			/* deleted copy constructor */
			logger(const self&) = delete;

			/* deleted move constructor */
			logger(self&&) = delete;

			/* destructor */
			~logger(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) noexcept -> self& {
				static self instance;
				return instance;
			}


			template <unsigned N, typename... Ts>
			auto _log(const char (&level)[N], const Ts&... args) -> void {

				// get timestamp
				sm::timestamp ts;

				_stream.append('\r', ts.data(), ' ', std::string_view{level}, ": ", args..., "\r\n");

				// write to stdout
				static_cast<void>(::write(STDOUT_FILENO,
										 _stream.data(),
										 _stream.size()));
			}


		public:

			// -- public static methods ---------------------------------------

			/* info */
			template <typename... Ts>
			static auto info(const Ts&... args) -> void {
				self::_shared()._log("\x1b[32mINFO\x1b[0m", args...);
			}

			/* warn */
			template <typename... Ts>
			static auto warn(const Ts&... args) -> void {
				self::_shared()._log("\x1b[33mWARN\x1b[0m", args...);
			}

			/* debug */
			template <typename... Ts>
			static auto debug(const Ts&... args) -> void {
				self::_shared()._log("\x1b[34mDEBG\x1b[0m", args...);
			}

			/* error */
			template <typename... Ts>
			static auto error(const Ts&... args) -> void {
				self::_shared()._log("\x1b[31mERRO\x1b[0m", args...);
			}

			/* signal */
			template <typename... Ts>
			static auto signal(const Ts&... args) -> void {
				self::_shared()._log("\x1b[35mSIGN\x1b[0m", args...);
			}

	}; // class logger

} // namespace sm

#endif // logger_hpp
