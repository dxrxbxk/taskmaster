#ifndef stream_hpp
#define stream_hpp

#include "types.hpp"
#include "utils/limits.hpp"
#include "type_traits/type_traits.hpp"
#include "memory/memcpy.hpp"
#include "string/strlen.hpp"

#include <iostream>

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


			template <typename T> requires sm::is_integral<T>
			auto _append(const T& value, size_t& offset) -> void {

				constexpr auto size = sm::limits<T>::digits() + sm::is_signed<T>;

				char buffer[size];
				char* ptr = buffer + size - 1U;

				T num = value;

				std::cout << "start num: " << num << std::endl;


				//*ptr = static_cast<char>((num % 10) ^ 0x30);




				do {

					// get digit
					T digit = num % 10;

					std::cout << "digit: " << digit << std::endl;

					//if (digit < 0) {
					//	digit = -digit;
					//}

					*ptr = static_cast<char>(digit + '0');

					//std::cout << "digit: " << (int)*ptr << std::endl;

					// divide by base
					num /= 10U;

					std::cout << "num: " << num << std::endl;

					// decrement digits
					--ptr;

				// check for zero
				} while (num != 0U);


				if constexpr (sm::is_signed<T>) {

					// check for negative
					if (value < 0) {
						*ptr = '-';
					} else {
						++ptr;
					}
				}

				// len
				const sm::usize len = (sm::usize)(buffer + size - ptr);

				// copy buffer to buffer
				_append_impl(ptr, len, offset);
			}


			auto _append(const char* str, size_t& offset) -> void {
				_append_impl(str, sm::strlen(str), offset);
			}

			auto _append(const char& ch, size_t& offset) -> void {
				_append_impl(&ch, 1U, offset);
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
			auto append(const Ts&... args) -> void {

				size_t offset = 0;

				(_append(args, offset), ...);
				_buffer[offset] = '\0';
			}


	}; // class stream

} // namespace sm

#endif // stream_hpp
