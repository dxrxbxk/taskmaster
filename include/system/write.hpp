#ifndef write_hpp
#define write_hpp

#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	struct stdin final {
		enum { descriptor = STDIN_FILENO };
	};
	struct stdout final {
		enum { descriptor = STDOUT_FILENO };
	};
	struct stderr final {
		enum { descriptor = STDERR_FILENO };
	};


	/* write */
	template <typename T = sm::stdout, unsigned int N>
	auto write(const char (&buffer)[N]) noexcept -> void {

		static_cast<void>(::write(T::descriptor, buffer, N));
	}

} // namespace sm

#endif // write_hpp
