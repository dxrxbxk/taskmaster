#ifndef is_integral_hpp
#define is_integral_hpp


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


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

#endif // is_integral_hpp
