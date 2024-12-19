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


	// -- I S  F L O A T I N G  P O I N T -------------------------------------

	/* is float */
	template <typename T>
	concept is_floating_point = (sm::is_same<T, float>
							  || sm::is_same<T, double>
							  || sm::is_same<T, long double>);


	// -- I S  A R I T H M E T I C --------------------------------------------

	/* is arithmetic */
	template <typename T>
	concept is_arithmetic = (sm::is_integral<T> || sm::is_floating_point<T>);


	// -- I S  S I G N E D ----------------------------------------------------

	/* is signed */
	template <typename T>
	concept is_signed = sm::is_arithmetic<T> && (T(-1) < T(0));

	/* conditional */
	template <bool B, class T, class F>
	struct conditional { using type = T; };

	template <class T, class F>
	struct conditional<false, T, F> { using type = F; };

	template <bool B, class T, class F>
	using conditional_t = typename conditional<B, T, F>::type;


	/* make unsigned */
	template <typename T>
	struct make_unsigned {
		using type = T;
	};

	template <typename T>
	using make_unsigned_t = typename make_unsigned<T>::type;

	template <>
	struct make_unsigned<signed char> {
		using type = unsigned char;
	};

	template <>
	struct make_unsigned<char> {
		using type = unsigned char;
	};

	template <>
	struct make_unsigned<short> {
		using type = unsigned short;
	};

	template <>
	struct make_unsigned<int> {
		using type = unsigned int;
	};

	template <>
	struct make_unsigned<long> {
		using type = unsigned long;
	};

	template <>
	struct make_unsigned<long long> {
		using type = unsigned long long;
	};

	/* remove reference */
	template <typename T>
	struct remove_reference {
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&> {
		using type = T;
	};

	template <typename T>
	struct remove_reference<T&&> {
		using type = T;
	};

	template <typename T>
	using remove_reference_t = typename remove_reference<T>::type;


	/* forward */
	template <typename T>
	constexpr T&& forward( remove_reference_t<T>& t ) noexcept {
		return static_cast<T&&>(t);
	}

	template <typename T>
	constexpr T&& forward( remove_reference_t<T>&& t ) noexcept {
		return static_cast<T&&>(t);
	}

	/* move */
	template <typename T>
	constexpr remove_reference_t<T>&& move(T&& t) noexcept {
		return static_cast<remove_reference_t<T>&&>(t);
	}

} // namespace sm

#endif // is_same_hpp
