#ifndef program_hpp
#define program_hpp

#include "core/process.hpp"
#include "core/profile.hpp"
#include "resources/shared_ptr.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program2 final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program2;


			// -- private members ---------------------------------------------

			/* profile */
			sm::shared_ptr<sm::profile> _profile;

			/* processes */
			std::vector<sm::process> _processes;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			program2(void) = delete;

			/* profile constructor */
			program2(sm::shared_ptr<sm::profile>&&);

			/* deleted copy constructor */
			program2(const self&) = delete;

			/* deleted move constructor */
			program2(self&&) = delete;

			/* destructor */
			~program2(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public accessors --------------------------------------------

			/* profile */
			auto profile(void) noexcept -> sm::profile&;

			/* processes */
			auto processes(void) noexcept -> std::vector<sm::process>&;

	}; // class program

} // namespace sm

#endif // program_hpp
