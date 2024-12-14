#ifndef stream_hpp
#define stream_hpp

#include "common/types.hpp"
#include "common/type_traits/is_integral.hpp"
#include "common/memory/memcpy.hpp"
#include "common/string/strlen.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class stream final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::stream;

			char* _buffer;
			sm::usize _size;


		public:

			stream(char* buffer, const sm::usize& size)
			: _buffer{buffer}, _size{size} {
			}


		private:

			template <typename T> requires sm::is_integral<T> // and not sm::is_bool<T>
			static consteval auto _max(void) noexcept -> T {

				// number of bits in type
				constexpr T bits = static_cast<T>((sizeof(T) * 8U) - 0U/*sm::is_signed<T>*/);

				T count = 0;

				// loop through bits
				for (T i = 0; i < bits; ++i)
					// add shifted bit to max
					count |= static_cast<T>(static_cast<T>(1) << i);

				return count;
			}

			template <typename T>
			static consteval auto _digits(void) -> sm::usize {

				// max value of type
				auto value = self::_max<T>();

				// number of digits
				sm::usize digits = 0U;

				do {

					// increment digits
					++digits;

				// divide max by base
				} while (value /= 10);

				return digits;
			}


			template <typename T> requires sm::is_integral<T> // unsigned
			auto _append(const T& num, size_t& offset) -> void {

				char buffer[self::_digits<T>() /* maybe + 1U (for sign) */];

				// number of digits
				sm::usize digits = sizeof(buffer) - 1U;

				do {

					// get digit
					buffer[digits] = static_cast<char>((num % 10U) ^ 0x30U);

					// divide by base
					num /= 10U;

					// decrement digits
					--digits;

				// check for zero
				} while (num != 0U);

				// copy buffer to buffer
				_append_impl(buffer + digits + 1U, sizeof(buffer) - digits - 1U, offset);
			}


			auto _append(const char* str, size_t& offset) -> void {
				_append_impl(str, sm::strlen(str), offset);
			}

			auto _append(const char& ch, size_t& offset) -> void {

				if (offset + 2U >= _size)
					return;

				_buffer[offset] = ch;
				++offset;
			}

			auto _append(const bool& b, size_t& offset) -> void {
				_append(b ? "true" : "false", offset);
			}

			template <size_t N>
			auto _append(const char (&str)[N], size_t& offset) -> void {
				_append_impl(str, N - 1U, offset);
			}


			auto _append_impl(const char* str, size_t len, size_t& offset) -> void {
				sm::memcpy(_buffer + offset, str, len);
				offset += len;
			}


		public:
			template <typename... Ts>
			auto print(const Ts&... args) -> void {

				size_t offset = 0;

				unsigned i = 0U;
				//(_append(args, offset), ...);
				_buffer[offset] = '\0';
			}


	}; // class stream

} // namespace sm

#endif // stream_hpp
