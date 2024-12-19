#ifndef OVERFLOW_HPP
# define OVERFLOW_HPP

# include "config.hpp"
# include <type_traits>
# include "types.hpp"
# include "utils/limits.hpp"

namespace sm {

template <typename IntegerType>
struct position_of_sign_bit {
	static_assert(std::is_integral_v<IntegerType>,
		"position_of_sign_bit: IntegralType must be an integral type");

	static const std::size_t value = sizeof(IntegerType) * 8 - 1;
};

template <typename T>
constexpr T binary_complement(T value) noexcept {
	return ~value;
}

template <typename T>
constexpr bool has_sign_bit(T value) noexcept {
	return bool(std::make_unsigned_t<T>(value) >> position_of_sign_bit<T>::value);
}

template <typename T>
constexpr bool valid_add(T a, T b) noexcept {
#if sm_has_builtin(__builtin_add_overflow)
	T dummy;
	return __builtin_add_overflow(a, b, &dummy);
#else
	std::make_unsigned_t<T> ua = a;
	std::make_unsigned_t<T> ub = b;
	std::make_unsigned_t<T> result = ua + ub;

	return std::is_signed_v<T>
		?  has_sign_bit(binary_complement(T((result ^ a) & (result ^ b))))
		: binary_complement(a) >= b;
#endif
}

template <typename T>
constexpr bool valid_sub(T a, T b) noexcept {
#if sm_has_builtin(__builtin_sub_overflow)
	T dummy;
	return __builtin_sub_overflow(a, b, &dummy);
#else
	std::make_unsigned_t<T> ua = a;
	std::make_unsigned_t<T> ub = b;
	std::make_unsigned_t<T> result = ua - ub;

	return std::is_signed_v<T>
		?  has_sign_bit(binary_complement(T((result ^ a) & (a ^ b))))
		: a >= b;

#endif
}

template <typename T, bool IsSigned = std::is_signed_v<T>>
struct is_mul_overflow;

template <typename T>
struct is_mul_overflow<T, true> {
	static constexpr bool run(T a, T b) noexcept {
		constexpr T max = sm::limits<T>::max();
		constexpr T min = sm::limits<T>::min();

		if (a == 0 || b == 0) {
			return true;
		}

		if (a > 0) {
			return a > 0 ? a <= max / b : a >= min / b;
		}

		return a > 0 ? a >= min / b : a <= max / b;

	}
};

template <typename T>
struct is_mul_overflow<T, false> {
	static constexpr bool run(T a, T b) noexcept {
		return b == 0 || a <= sm::limits<T>::max() / b;
	}
};

template <typename T>
constexpr bool valid_mul(T a, T b) noexcept {
#if sm_has_builtin(__builtin_mul_overflow)
	T dummy;
	return __builtin_mul_overflow(a, b, &dummy);
#else

	return is_mul_overflow<T>::run(a, b);
#endif
}

} // namespace sm

#endif // OVERFLOW_HPP
