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

			/* group id */
			::pid_t _gid;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			program2(void) = delete;

			/* profile constructor */
			program2(sm::shared_ptr<sm::profile>&&);

			/* deleted copy constructor */
			program2(const self&) = delete;

			/* move constructor */
			program2(self&&) noexcept = default;

			/* destructor */
			~program2(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* profile */
			auto profile(void) noexcept -> sm::profile&;

			/* processes */
			auto processes(void) noexcept -> std::vector<sm::process>&;

			/* group id */
			auto group_id(void) const noexcept -> const ::pid_t&;


			// -- public modifiers --------------------------------------------

			/* group id */
			auto group_id(const ::pid_t&) noexcept -> void;

	}; // class program

} // namespace sm

#endif // program_hpp
