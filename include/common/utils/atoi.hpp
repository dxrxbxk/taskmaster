#ifndef ATOI_HPP
#define ATOI_HPP

#include <type_traits>
#include <limits>

#include "common/diagnostics/exception.hpp"
#include <iostream>

namespace sm {

struct dec {
	static constexpr auto value = 10;
};

struct hex {
	static constexpr auto value = 16;
};

struct oct {
	static constexpr auto value = 8;
};

struct bin {
	static constexpr auto value = 2;
};

template <typename T, typename U=sm::dec> requires (std::is_unsigned_v<T> && not std::is_same_v<T, bool>)
auto atoi(const char* str) -> T {

	T result = 0;


	while (*str) {

		if ((*str ^ '0') > 9)
			throw sm::runtime_error{"not a number"};

		constexpr auto max = std::numeric_limits<T>::max();

		if (result > (max / static_cast<T>(U::value)))
			throw sm::runtime_error{"overflow"};

		result *= U::value;
		const auto digit = static_cast<T>(*str - '0');

		if (result > (max - digit))
			throw sm::runtime_error{"overflow"};

		result += digit; ++str;
	}

	return result;
}

template <typename T, typename U=sm::dec> requires (std::is_signed_v<T> && not std::is_same_v<T, bool>)
auto atoi(const char *str) -> T {
	T result = 0;

	if (*str == '-') {
		while (*++str) {

			if ((*str ^ '0') > 9)
				throw sm::runtime_error{"atoi signed: invalid character"};

			constexpr auto min = std::numeric_limits<T>::min();

			if (result < (min / static_cast<T>(U::value)))
				throw sm::runtime_error{"overflow"};

			result *= U::value;

			const auto digit = static_cast<T>(*str - '0');

			if (result < (min + digit))
				throw sm::runtime_error{"overflow"};

			result -= digit;
		}
	} else {
		while (*str) {

			if ((*str ^ '0') > 9)
				throw sm::runtime_error{"atoi signed: invalid character"};

			constexpr auto max = std::numeric_limits<T>::max();

			if (result > (max / static_cast<T>(10)))
				throw sm::runtime_error{"overflow"};

			result *= 10;

			const auto digit = static_cast<T>(*str - '0');

			if (result > (max - digit))
				throw sm::runtime_error{"overflow"};

			result += digit;

			++str;
		}
	}

	return result;
}

template <typename T> requires std::is_same_v<T, bool>
auto atoi(const char* str) -> bool {
	if ((str[0] == 't' && str[1] == 'r' && str[2] == 'u' && str[3] == 'e' && str[4] == '\0')
	 || (str[0] == '1' && str[1] == '\0'))
		return true;

	if ((str[0] == 'f' && str[1] == 'a' && str[2] == 'l' && str[3] == 's' && str[4] == 'e' && str[5] == '\0')
	 || (str[0] == '0' && str[1] == '\0'))
		return false;

	throw sm::runtime_error{"bool: invalid value"};
}

} // namespace sm

#endif // ATOI_HPP
