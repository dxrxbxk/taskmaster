#ifndef exception_hpp
#define exception_hpp

#include "memory/memcpy.hpp"
#include "string/strlen.hpp"

#include <string.h>
#include <errno.h>

#include <stdexcept>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* alias to std::exception */
	using exception = std::exception;


	// -- E X C E P T I O N ---------------------------------------------------

	//class exception {
	//
	//
	//	public:
	//
	//		// -- public lifecycle --------------------------------------------
	//
	//		/* default constructor */
	//		exception(void) noexcept = default;
	//
	//		/* copy constructor */
	//		exception(const exception&) noexcept = default;
	//
	//		/* move constructor */
	//		exception(exception&&) noexcept = default;
	//
	//		/* destructor */
	//		virtual ~exception(void) noexcept = default;
	//
	//
	//		// -- public assignment operators ---------------------------------
	//
	//		/* copy assignment operator */
	//		auto operator=(const exception&) noexcept -> exception& = default;
	//
	//		/* move assignment operator */
	//		auto operator=(exception&&) noexcept -> exception& = default;
	//
	//
	//		// -- public interface --------------------------------------------
	//
	//		/* what */
	//		virtual auto what(void) const noexcept -> const char* = 0;
	//
	//}; // class exception


	// -- S Y S T E M  E R R O R ----------------------------------------------

	class system_error final : public sm::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::system_error;


			// -- private members ---------------------------------------------

			/* errno */
			int _errno;

			/* buffer */
			char _buffer[4096U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			system_error(void) noexcept
			: sm::system_error{"unknown location"} {
			}

			/* where constructor */
			template <unsigned N>
			system_error(const char (&where)[N]) noexcept
			: _errno{errno} /* _buffer{} uninitialized */ {

				// buffer size
				constexpr auto buffer_size = sizeof(_buffer);

				// get errno string error
				const char* err_str = ::strerror(errno);

				// check error
				if (err_str == nullptr)
					err_str = "unknown error";

				// where length
				size_t w_len = N - (where[N - 1U] == '\0' ? 1U : 0U);

				// error length
				size_t err_len = strlen(err_str);

				// check out of bounds
				if (err_len + w_len + 3U > buffer_size)
					err_len = buffer_size - w_len - 3U;

				// offset pointer
				char* ptr = _buffer;

				// copy where
				sm::memcpy(ptr, where, w_len);
				ptr += w_len;

				// copy separator
				sm::memcpy(ptr, ": ", 2);
				ptr += 2;

				// copy error
				sm::memcpy(ptr, err_str, err_len);
				ptr += err_len;

				// null terminate
				*ptr = '\0';
			}

			/* copy constructor */
			system_error(const self&) noexcept = default;

			/* move constructor */
			system_error(self&&) noexcept = default;

			/* destructor */
			~system_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override {
				return _buffer;
			}


			// -- public accessors --------------------------------------------

			/* code */
			auto code(void) const noexcept -> int {
				return _errno;
			}

	}; // class system_error


	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error final : public sm::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::runtime_error;


			// -- private members ---------------------------------------------

			/* what */
			char _buffer[4096U];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runtime_error(void) noexcept {
				memcpy(_buffer, "unknown error", 14);
			}


			void _append(const char* str, sm::usize& offset) {

				const auto len = sm::strlen(str);

				char* ptr = _buffer + offset;

				sm::memcpy(ptr, str, len);

				ptr += len;

				sm::memcpy(ptr, " ", 1U);

				offset += len + 1U;
			}

			template<typename... Ts>
			runtime_error(Ts&&... args) noexcept {
				size_t offset = 0;
				unsigned i = 0U;
				(_append(args, offset), ...);
				_buffer[offset] = '\0';
			}

			/* copy constructor */
			runtime_error(const self&) noexcept = default;

			/* move constructor */
			runtime_error(self&&) noexcept = default;

			/* destructor */
			~runtime_error(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* what */
			auto what(void) const noexcept -> const char* override {
				return _buffer;
			}

	}; // class runtime_error


	class exit final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::exit;


			// -- private members ---------------------------------------------

			/* status */
			int _status;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			exit(void) = delete;

			/* status constructor */
			exit(const int& status) noexcept
			: _status{status} {
			}

			/* copy constructor */
			exit(const self&) noexcept = default;

			/* move constructor */
			exit(self&&) noexcept = default;

			/* destructor */
			~exit(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* status */
			auto status(void) const noexcept -> int {
				return _status;
			}

	}; // class exit

} // namespace sm

#endif // exception_hpp
