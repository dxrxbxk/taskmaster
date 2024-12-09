#ifndef exception_hpp
#define exception_hpp

#include <string.h>
#include <errno.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	class exception {
		public:
			exception() noexcept = default;
			exception(exception&&) noexcept = default;
			exception(const exception&) noexcept = default;
			exception& operator=(exception&&) noexcept = default;
			exception& operator=(const exception&) noexcept = default;
			virtual ~exception() noexcept = default;
			virtual const char* what() const noexcept = 0;
	};

	class errno_error : public exception {
		public:
			template <size_t N>
			errno_error(const char (&where)[N]) noexcept {
				constexpr auto buffer_size = sizeof(_buffer);

				const char* err_str = strerror(errno);
				if (err_str == nullptr) {
					err_str = "Unknown error";
				}

				size_t w_len = N - (where[N - 1] == '\0' ? 1 : 0);

				size_t err_len = strlen(err_str);
				if(err_len + w_len + 3 > buffer_size) {
					err_len = buffer_size - w_len - 3;
				}

				char *ptr = _buffer;

				memcpy(ptr, where, w_len);
				ptr += w_len;
				memcpy(ptr, ": ", 2);
				ptr += 2;

				memcpy(ptr, err_str, err_len);
				ptr += err_len;
				*ptr = '\0';
			}

			errno_error() noexcept : errno_error("Unknown location") {
			}

			const char* what() const noexcept override {
				return _buffer;
			}
		private:
			char _buffer[4096];
	};

	class runtime_error : public exception {
		public:
			runtime_error(const char* what) noexcept {
				_buffer = what;
			}

			const char* what() const noexcept override {
				return _buffer;
			}
		private:
			const char* _buffer;
	};

} // namespace sm

#endif // exception_hpp

