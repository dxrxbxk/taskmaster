#ifndef type_traits_hpp
#define type_traits_hpp


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- I S  S A M E --------------------------------------------------------

	namespace impl {

		template <typename T, typename U>
		struct is_same final {
			static constexpr bool value = false;
		};

		template <typename T>
		struct is_same<T, T> final {
			static constexpr bool value = true;
		};

	} // namespace impl


	/* is same */
	template <typename T, typename U>
	concept is_same = sm::impl::is_same<T, U>::value;


	// -- I S  B O O L --------------------------------------------------------

	/* is bool */
	template <typename T>
	concept is_bool = sm::is_same<T, bool>;



	// -- I S  I N T E G R A L ------------------------------------------------

	/* is integral */
	template <typename T> // T* parameter excludes reference types
	concept is_integral = requires (T t, T* p, void (*f)(T)) {
		// exclude class types
        reinterpret_cast<T>(t);

		// exclude enumeration types
        f(0);

		// exclude everything not yet excluded but integral types
        p + t;
    };

	// cppreference.com implementation

} // namespace sm

#endif // is_same_hpp
