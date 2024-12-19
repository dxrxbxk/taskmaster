#ifndef stream_hpp
#define stream_hpp

#include "types.hpp"

#include "utils/limits.hpp"
#include "type_traits/type_traits.hpp"
#include "memory/memcpy.hpp"
#include "memory/malloc.hpp"
#include "string/strlen.hpp"

#include <iostream>

// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class stream final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::stream;


			// -- private members ---------------------------------------------

			/* buffer */
			char* _buffer;

			/* capacity */
			sm::usize _capacity;

			/* size */
			sm::usize _size;


		public:

			stream(void) noexcept
			: _buffer{nullptr},
			  _capacity{0U},
			  _size{0U} {
			}

			/* deleted copy constructor */
			stream(const self&) = delete;

			/* deleted move constructor */
			stream(self&&) = delete;

			/* destructor */
			~stream(void) noexcept {

				if (_buffer == nullptr)
					return;

				sm::free(_buffer);
			}


		private:


			template <typename T> requires (sm::is_integral<T> && !sm::is_same<T, char>)
			auto _append(const T& value, size_t& offset) -> void {

				constexpr auto size = sm::limits<T>::digits() + sm::is_signed<T>;

				char buffer[size];
				char* ptr = buffer + size - 1U;

				T num = value;


				// extract digit
				T digit = num % static_cast<T>(10);

				if constexpr (sm::is_signed<T>) {
					// check for negative
					digit = (value < 0) ? -digit : digit;
				}

				*ptr = static_cast<char>(digit + '0');

				// divide by base
				num /= static_cast<T>(10);

				// decrement digits
				--ptr;

				if constexpr (sm::is_signed<T>) {
					num = (value < 0) ? -num : num;
				}

				while (num != 0U) {
					*ptr = static_cast<char>((num % static_cast<T>(10)) + '0');
					num /= static_cast<T>(10);
					--ptr;
				}

				if constexpr (sm::is_signed<T>) {
					// check for negative
					if (value < 0)
						*ptr = '-';
					else
						++ptr;
				}

				// len
				const sm::usize len = static_cast<sm::usize>(buffer + size - ptr);

				// copy buffer to buffer
				_append_impl(ptr, len, offset);
			}



			auto _append(const char& ch, size_t& offset) -> void {
				_append_impl(&ch, 1U, offset);
			}

			auto _append(const std::string& str, size_t& offset) -> void {
				_append_impl(str.data(), str.size(), offset);
			}

			auto _append(const std::string_view& str, size_t& offset) -> void {
				_append_impl(str.data(), str.size(), offset);
			}

			template <size_t N>
			auto _append(const char (&str)[N], size_t& offset) -> void {
				_append_impl(str, N - 1U, offset);
			}

			auto _append(const bool& b, size_t& offset) -> void {
				(b == true) ? _append("true", offset)
							: _append("false", offset);
			}

			template <typename T> requires (sm::is_integral<T> && !sm::is_same<T, char>)
			auto _compute_size(const T& value) -> sm::usize {
				constexpr auto size = sm::limits<T>::digits() + sm::is_signed<T>;
				return size;
			}

			auto _compute_size(const char& ch) -> sm::usize {
				return 1U;
			}

			auto _compute_size(const std::string& str) -> sm::usize {
				return str.size();
			}

			auto _compute_size(const std::string_view& str) -> sm::usize {
				return str.size();
			}

			template <size_t N>
			auto _compute_size(const char (&str)[N]) -> sm::usize {
				return N - 1U; // assume null-terminated
			}

			auto _compute_size(const bool& b, size_t& offset) -> sm::usize {
				return (b == true) ? 4U : 5U;
			}


			auto _append_impl(const char* str, size_t len, size_t& offset) -> void {
				sm::memcpy(_buffer + offset, str, len);
				offset += len;
			}


		public:

			template <typename... Ts>
			auto append(const Ts&... args) -> void {

				const auto size = (self::_compute_size(args) + ...);

				if (_capacity < size) {

					char* buffer = sm::malloc<char>(size + 1U);
					sm::free(_buffer);
					_buffer = buffer;
					_capacity = size;
				}


				sm::usize offset = 0;

				(self::_append(args, offset), ...);

				_buffer[offset] = '\0';

				_size = offset;
			}

			auto data(void) const noexcept -> const char* {
				return _buffer;
			}

			auto size(void) const noexcept -> sm::usize {
				return _size;
			}

	}; // class stream

} // namespace sm

/*
auto operator new(size_t) -> void* {
	return sm::malloc<int>(1U);
}
*/

#endif // stream_hpp
