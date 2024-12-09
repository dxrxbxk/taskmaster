#ifndef exception_hpp
#define exception_hpp

#include <string.h>
#include <errno.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E X C E P T I O N ---------------------------------------------------

	class exception {


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			exception(void) noexcept = default;

			/* copy constructor */
			exception(const exception&) noexcept = default;

			/* move constructor */
			exception(exception&&) noexcept = default;

			/* destructor */
			virtual ~exception(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const exception&) noexcept -> exception& = default;

			/* move assignment operator */
			auto operator=(exception&&) noexcept -> exception& = default;


			// -- public interface --------------------------------------------

			/* what */
			virtual auto what(void) const noexcept -> const char* = 0;

	}; // class exception


	// -- S Y S T E M  E R R O R ----------------------------------------------

	class system_error : public sm::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::system_error;


			// -- private members ---------------------------------------------

			/* buffer */
			char _buffer[4096];


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			system_error(void) noexcept
			: sm::system_error{"unknown location"} {
			}

			/* where constructor */
			template <unsigned N>
			system_error(const char (&where)[N]) noexcept
			/* : _buffer{} uninitialized */ {

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
				if(err_len + w_len + 3U > buffer_size)
					err_len = buffer_size - w_len - 3U;

				// offset pointer
				char* ptr = _buffer;

				// copy where
				memcpy(ptr, where, w_len);
				ptr += w_len;

				// copy separator
				memcpy(ptr, ": ", 2);
				ptr += 2;

				// copy error
				memcpy(ptr, err_str, err_len);
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

	}; // class system_error


	// -- R U N T I M E  E R R O R --------------------------------------------

	class runtime_error : public sm::exception {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::runtime_error;


			// -- private members ---------------------------------------------

			/* what */
			const char* _what;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			runtime_error(void) noexcept
			: _what{"unknown error"} {
			}

			/* what constructor */
			runtime_error(const char* what) noexcept
			: _what{what != nullptr ? what : "unknown error"} {
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
				return _what;
			}

	}; // class runtime_error

} // namespace sm

#endif // exception_hpp
