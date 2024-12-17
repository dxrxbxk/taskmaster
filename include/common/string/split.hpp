#ifndef split_hpp
#define split_hpp

#include "common/types.hpp"
#include <string>
#include <vector>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	/* is charset */
	static auto _is_charset(const char& ch, const char* charset) -> bool {

		// check if charset is null
		if (charset == nullptr) 
			return false;

		// loop through charset
		while (*charset != '\0') {
			// check if character is in charset
			if (ch == *charset)
				return true;
			++charset;
		}
		return false;
	}

	/* split */
	inline auto split(const std::string& str, const std::string& sep) -> std::vector<std::string> {

		// check if string is not null
		if (str.empty())
			return {};

		std::vector<std::string> vec;

		const char* ptr = str.data();

		// loop through string characters
		for (sm::usize z = 0U, x = 0U; ptr[x]; x += z, z = 0U) {

			// skip separator characters
			while (_is_charset(ptr[x], sep.data()))
				++x;

			// count non-separator characters
			while (ptr[x + z] && !_is_charset(ptr[x + z], sep.data()))
				++z;

			// emplace string if z is not zero
			if (z)
				vec.emplace_back(&(ptr[x]), z);

		}

		// return the vector
		return vec;
	}

} // namespace sm

#endif // split_hpp
