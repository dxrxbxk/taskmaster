#ifndef IS_DIR_HPP
# define IS_DIR_HPP

# include <sys/stat.h>

# include "common/diagnostics/exception.hpp"

namespace sm {

	/* is_dir */
	auto inline is_dir(const char* path) -> bool {

		struct ::stat st;

		if (::stat(path, &st) == -1) {
			if (errno == ENOENT) 
				return false;
			else
				throw sm::system_error{"stat"};
		}

		return S_ISDIR(st.st_mode);
	}

	/* is_command */
	auto inline is_command(const char* path) -> bool {

		struct ::stat st;

		if (::stat(path, &st) == -1) {
			if (errno == ENOENT) 
				return false;
			else
				throw sm::system_error{"stat"};
		}

		return S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR);
	}

	//auto inline is_file(const char* path) -> bool {
	//
	//	struct ::stat st;
	//
	//	if (::stat(path, &st) == -1)
	//		return false;
	//
	//	return S_ISREG(st.st_mode);
	//}
}

#endif
