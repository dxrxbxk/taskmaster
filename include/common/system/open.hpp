#ifndef open_hpp
#define open_hpp

#include <fcntl.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* open */
	auto open(const char*, const int&) -> int;

	/* open */
	auto open(const char*, const int&, const ::mode_t) -> int;

} // namespace sm

#endif // open_hpp
