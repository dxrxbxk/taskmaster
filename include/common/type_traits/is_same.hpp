#ifndef is_same_hpp
#define is_same_hpp


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- I S  S A M E --------------------------------------------------------

	template <typename T, typename U>
	struct is_same final {
		static constexpr bool value = false;
	};

	template <typename T>
	struct is_same<T, T> final {
		static constexpr bool value = true;
	};

	template <typename T>
	struct test final {

		static constexpr bool value = requires(T t) {
			{ t.value };
		};
	};

} // namespace sm

#endif // is_same_hpp
