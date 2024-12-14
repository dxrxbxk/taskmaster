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

template <typename T, typename U=sm::dec> requires std::is_unsigned_v<T>
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

template <typename T, typename U=sm::dec> requires std::is_signed_v<T>
auto atoi(const char *str) -> T {
	T result = 0;

	if (*str == '-') {
		while (*++str) {
			if ((*str ^ '0') > 9)
				throw sm::runtime_error{"atoi signed: invalid character"};
			constexpr auto min = std::numeric_limits<T>::min();
			if (result < (min / static_cast<T>(U::value)))
				throw sm::runtime_error{"atoi signed: underflow: (min / ", U::value, " )"};
			result *= U::value;
			const auto digit = static_cast<T>(*str - '0');
			if (result < (min + digit))
				throw sm::runtime_error{"atoi signed: underflow"};
			result -= digit;
		}
	} else {
		while (*str) {
			if ((*str ^ '0') > 9)
				throw sm::runtime_error{"atoi signed: invalid character"};
			constexpr auto max = std::numeric_limits<T>::max();
			if (result > (max / static_cast<T>(10)))
				throw sm::runtime_error{"atoi signed: overflow: (max / 10)"};
			result *= 10;
			const auto digit = static_cast<T>(*str - '0');
			if (result > (max - digit))
				throw sm::runtime_error{"atoi signed: overflow"};
			result += digit;
			++str;
		}
	}

	return result;
}

} // namespace sm

#endif // ATOI_HPP
