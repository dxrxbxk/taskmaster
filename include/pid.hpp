#ifndef pid_hpp
#define pid_hpp

#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P I D ---------------------------------------------------------------

	class pid final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::pid;


			// -- private members ---------------------------------------------

			/* pid */
			::pid_t _pid;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			pid(void) noexcept
			: _pid{::getpid()} {
			}


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> self& {
				static self instance;
				return instance;
			}


		public:

			// -- public static methods ---------------------------------------

			/* get */
			static auto get(void) noexcept -> ::pid_t {
				return self::_shared()._pid;
			}

	}; // class pid


} // namespace sm

#endif // pid_hpp
