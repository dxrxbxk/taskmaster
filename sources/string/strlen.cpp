#include "string/strlen.hpp"
#include "config.hpp"

#if !sm_has_builtin(__builtin_strlen)
#	include <string.h>
#endif


/* strlen */
auto sm::strlen(const char* const str) noexcept -> sm::usize {

	#if sm_has_builtin(__builtin_strlen)
		return __builtin_strlen(str);
	#else
		return ::strlen(str);
	#endif
}
