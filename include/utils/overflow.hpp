#ifndef OVERFLOW_HPP
# define OVERFLOW_HPP

# include "config.hpp"
# include <type_traits>
# include "types.hpp"
# include "utils/limits.hpp"

# include "diagnostics/exception.hpp"

namespace sm {

/* Returns the position of the sign bit in the given integer type */
template <typename IntegerType>
struct position_of_sign_bit {
	static const std::size_t value = sizeof(IntegerType) * 8 - 1;
};

/* Returns the binary complement of the given value */
template <typename T>
constexpr T binary_complement(T value) noexcept {
	return ~value;
}

/* Returns true if the given value has the sign bit set */
template <typename T>
constexpr bool has_sign_bit(T value) noexcept {
	return bool(sm::make_unsigned_t<T>(value) >> position_of_sign_bit<T>::value);
}

/* Checks if the built-in __builtin_add_overflow is available 
 * valid_add checks if the addition of a and b will overflow
 * casting the the values in ua and ub in case they are signed (to avoid undefined behavior)
 * if its signed, we check if the sign bit of the result is different from the sign bit of a and b
 * if its unsigned, we check if the binary complement of a is greater than b
 * returns true if the addition of a and b will overflow
 */
template <typename T>
constexpr bool valid_add(T a, T b) noexcept {
#if sm_has_builtin(__builtin_add_overflow)
	T dummy;
	return __builtin_add_overflow(a, b, &dummy);
#else
	sm::make_unsigned_t<T> ua = a;
	sm::make_unsigned_t<T> ub = b;
	sm::make_unsigned_t<T> result = ua + ub;

	return std::is_signed_v<T>
		?  has_sign_bit(binary_complement(T((result ^ a) & (result ^ b))))
		: binary_complement(a) >= b;
#endif
}

/* Checks if the built-in __builtin_sub_overflow is available
 * valid_sub checks if the subtraction of a and b will overflow
 * casting the the values in ua and ub in case they are signed (to avoid undefined behavior)
 * if its signed, we check if the sign bit of the result is different from the sign bit of a and b
 * if its unsigned, we check if the binary complement of a is greater than b
 * return true if the subtraction of a and b will overflow
 */
template <typename T>
constexpr bool valid_sub(T a, T b) noexcept {
#if sm_has_builtin(__builtin_sub_overflow)
	T dummy;
	return __builtin_sub_overflow(a, b, &dummy);
#else
	sm::make_unsigned_t<T> ua = a;
	sm::make_unsigned_t<T> ub = b;
	sm::make_unsigned_t<T> result = ua - ub;

	return std::is_signed_v<T>
		?  has_sign_bit(binary_complement(T((result ^ a) & (a ^ b))))
		: a >= b;

#endif
}

/* returns true if the multiplication of a and b will overflow */
template <typename T> requires sm::is_signed<T>
constexpr bool valid_mul_impl(T a, T b) noexcept {
		constexpr T max = sm::limits<T>::max();
		constexpr T min = sm::limits<T>::min();

		if (a == 0 || b == 0) {
			return true;
		}


		if (a > 0) {
			return b > 0 ? a <= max / b : b >= min / a;
		}

		/* if a is negative */

		return b > 0 ? a >= min / b : b >= max / a;

}

template <typename T> requires sm::is_unsigned<T>
constexpr bool valid_mul_impl(T a, T b) noexcept {
	return b == 0 || a <= sm::limits<T>::max() / b;
}

/* Checks if the built-in __builtin_mul_overflow is available
 * valid_mul checks if the multiplication of a and b will overflow
 */
template <typename T>
constexpr bool valid_mul(T a, T b) noexcept {
#if sm_has_builtin(__builtin_mul_overflow)
	T dummy;
	return __builtin_mul_overflow(a, b, &dummy);
#else
	return valid_mul_impl(a, b);
#endif
}

/* Special case for example when a = INT_MIN and b = -1 == INT_MAX + 1
 * the result of the division will be INT_MIN, which is not representable in a signed integer
 * so we return false
 */
template <typename T>
constexpr bool valid_div(T a, T b) noexcept {
	return b != 0 && !(std::is_signed_v<T> && a == sm::limits<T>::min() && b == T(-1));
}

/* returns true if the modulo of a and b will overflow */
template <typename T>
constexpr bool valid_mod(T a, T b) noexcept {
	return valid_mod_impl(a, b);
}

template <typename T> requires sm::is_signed<T>
constexpr bool valid_mod_impl(T a, T b) noexcept {
	return b != 0 && (a == sm::limits<T>::min() && b == T(-1));
};

template <typename T> requires sm::is_unsigned<T>
constexpr bool valid_mod_impl(T a, T b) noexcept {
	return b != 0;
};

} // namespace sm

#endif // OVERFLOW_HPP
