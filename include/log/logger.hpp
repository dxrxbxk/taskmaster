#ifndef logger_hpp
#define logger_hpp

#include "log/timestamp.hpp"
#include "diagnostics/exception.hpp"
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

			/* buffer */
			std::string _buffer;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			logger(void) noexcept
			: _buffer{} {
			}

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


			auto _log(const char* level, const char* message) noexcept -> void {

				// get timestamp
				sm::timestamp ts;

				_buffer.clear();
				_buffer.append(ts.data(), ts.size());
				_buffer.append(" ", 1);
				_buffer.append(level, sm::strlen(level));
				_buffer.append(": ", 2);
				_buffer.append(message, sm::strlen(message));
				_buffer.append("\r\n", 2);

				// write to stdout
				static_cast<void>(::write(STDOUT_FILENO,
							_buffer.data(),
							_buffer.size()));
			}


		public:

			// -- public static methods ---------------------------------------

			/* info */
			static auto info(const char* message) -> void {
				self::_shared()._log("\x1b[32mINFO\x1b[0m", message);
			}

			/* warn */
			static auto warn(const char* message) -> void {
				self::_shared()._log("\x1b[33mWARN\x1b[0m", message);
			}

			/* debug */
			static auto debug(const char* message) -> void {
				self::_shared()._log("\x1b[34mDEBG\x1b[0m", message);
			}

			/* error */
			static auto error(const char* message) -> void {
				self::_shared()._log("\x1b[31mERRO\x1b[0m", message);
			}

			/* signal */
			static auto signal(const char* message) -> void {
				self::_shared()._log("\x1b[35mSIGN\x1b[0m", message);
			}

	}; // class logger

} // namespace sm

#endif // logger_hpp
