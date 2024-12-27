#ifndef environment_hpp
#define environment_hpp

#include "containers/contiguous_cstr.hpp"
#include <unistd.h>

extern char **environ;

// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- E N V I R O N M E N T -----------------------------------------------

	class environment final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::environment;


			// -- private members ---------------------------------------------

			/* environment */
			sm::contiguous_cstr _env;

			static auto _shared(void) -> self& {
				static self instance;
				return instance;
			}

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			environment(void) {
				// get environment
				for (char **env = environ; *env != nullptr; ++env)
					_env.push(*env);
			}

		public:

			static auto copy(void) -> sm::contiguous_cstr {
				return self::_shared()._env;
			}


	}; // class environment

} // namespace sm

#endif // environment_hpp
