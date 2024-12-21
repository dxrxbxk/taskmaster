#ifndef rlimit_hpp
#define rlimit_hpp

#include <sys/resource.h>
#include "diagnostics/exception.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- R L I M I T ---------------------------------------------------------

	template <int R>
	class rlimit final {


		// -- assertions ------------------------------------------------------

		static_assert( /* cpu time per process */
					R == RLIMIT_CPU
					   /* file size */
				 || R == RLIMIT_FSIZE
					   /* data segment size */
				 || R == RLIMIT_DATA
					   /* stack size */
				 || R == RLIMIT_STACK
					   /* core file size */
				 || R == RLIMIT_CORE
					   /* address space (resident set size) */
				 || R == RLIMIT_AS
					   /* source compatibility alias */
				 || R == RLIMIT_RSS
					   /* locked-in-memory address space */
				 || R == RLIMIT_MEMLOCK
					   /* number of processes */
				 || R == RLIMIT_NPROC
					   /* number of open files */
				 || R == RLIMIT_NOFILE, "invalid resource");


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::rlimit<R>;


			// -- private members ---------------------------------------------

			/* limit */
			struct ::rlimit _limit;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			rlimit(void)
			/* : _limit{} uninitialized */ {

				// get the limit
				if (::getrlimit(R, &_limit) != 0)
					throw sm::system_error("getrlimit");
			}

			/* deleted copy constructor */
			rlimit(const self&) = delete;

			/* deleted move constructor */
			rlimit(self&&) = delete;

			/* destructor */
			~rlimit(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- private static methods --------------------------------------

			/* shared */
			static auto _shared(void) -> self& {
				static self instance;
				return instance;
			}


		public:

			// -- public static accessors -------------------------------------

			/* soft */
			static auto soft(void) -> ::rlim_t {
				return self::_shared()._limit.rlim_cur;
			}

			/* hard */
			static auto hard(void) -> ::rlim_t {
				return self::_shared()._limit.rlim_max;
			}


			// -- public static modifiers -------------------------------------

			/* soft */
			static auto soft(const ::rlim_t limit) -> void {

				// get the instance
				auto& rl = self::_shared()._limit;

				// assign the limit
				rl.rlim_cur = limit;

				// set the limit
				if (::setrlimit(R, &rl) != 0)
					throw sm::system_error("setrlimit");
			}

			/* hard */
			static auto hard(const ::rlim_t limit) -> void {

				// get the instance
				auto& rl = self::_shared()._limit;

				// assign the limit
				rl.rlim_max = limit;

				// set the limit
				if (::setrlimit(R, &rl) != 0)
					throw sm::runtime_error("failed to set the resource limit");
			}

	}; // class rlimit

} // namespace sm

#endif // rlimit_hpp
