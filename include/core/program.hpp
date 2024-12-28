#ifndef program_hpp
#define program_hpp

#include "core/process.hpp"
#include "core/profile.hpp"
#include "resources/shared_ptr.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program;


			// -- private members ---------------------------------------------

			/* profile */
			sm::shared_ptr<sm::profile> _profile;

			/* processes */
			std::vector<sm::process> _processes;

			/* hash */
			sm::usize _hash;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			program(void) = delete;

			/* profile constructor */
			program(sm::shared_ptr<sm::profile>&&);

			/* deleted copy constructor */
			program(const self&) = delete;

			/* move constructor */
			program(self&&) noexcept = default;

			/* destructor */
			~program(void) noexcept = default;


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

			/* status */
			auto status(void) const -> void;


			// -- public modifiers --------------------------------------------

			/* group id */
			auto group_id(const ::pid_t&) noexcept -> void;

			/* autostart */
			auto autostart(sm::monitor&) -> void;


			// -- public methods ----------------------------------------------

			/* hot swap */
			auto hot_swap(self&&) -> void;

	}; // class program

} // namespace sm

#endif // program_hpp
