#ifndef types_hpp
#define types_hpp

#include <stdint.h>
#include <sys/types.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* 8-bit unsigned integer */
	using u8   = ::uint8_t;

	/* 16-bit unsigned integer */
	using u16  = ::uint16_t;

	/* 32-bit unsigned integer */
	using u32  = ::uint32_t;

	/* 64-bit unsigned integer */
	using u64  = ::uint64_t;


	/* 8-bit signed integer */
	using i8   = ::int8_t;

	/* 16-bit signed integer */
	using i16  = ::int16_t;

	/* 32-bit signed integer */
	using i32  = ::int32_t;

	/* 64-bit signed integer */
	using i64  = ::int64_t;


	/* 32-bit floating point */
	using f32  = float;

	/* 64-bit floating point */
	using f64  = double;


	/* maximum unsigned integer */
	using umax = ::uintmax_t;

	/* maximum signed integer */
	using imax = ::intmax_t;


	/* unsigned size */
	using usize = ::size_t;

	/* signed size */
	using isize = ::ssize_t;

} // namespace sm

#endif // types_hpp
