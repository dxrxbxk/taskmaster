#ifndef core_affinity_hpp
#define core_affinity_hpp

#include "common/types.hpp"
#include "common/system/sysconf.hpp"
#include <sched.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C O R E  A F F I N I T Y --------------------------------------------

	class core_affinity final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::core_affinity;


			// -- private lifecycle -------------------------------------------

			/* deleted default constructor */
			core_affinity(void) = delete;


		public:

			// -- public static methods ---------------------------------------

			/* set */
			static auto set(sm::usize& ncores) -> void {

				::cpu_set_t cpuset{};

				// get maximum number of cores
				const auto& max_cores = self::max_cores();

				// limit number of cores
				ncores = max_cores < ncores ? max_cores : ncores;

				// set cpu set
				for (sm::usize i = 0U; i < ncores; ++i)
					CPU_SET(i, &cpuset);

				// set affinity
				if (::sched_setaffinity(0, sizeof(cpuset), &cpuset) != 0)
					throw sm::system_error{"sched_setaffinity"};
			}

			/* maximum number of cores */
			static auto max_cores(void) -> sm::usize {

				// get number of cores
				static const auto cores = static_cast<sm::usize>(
						sm::sysconf<_SC_NPROCESSORS_ONLN>());

				return cores;
			}

	}; // class core_affinity

} // namespace sm

#endif // core_affinity_hpp
