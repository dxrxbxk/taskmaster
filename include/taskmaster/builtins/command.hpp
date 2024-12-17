#ifndef command_hpp
#define command_hpp

#include "taskmaster/taskmaster.hpp"

#include <vector>
#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C O M M A N D -------------------------------------------------------

	class command {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::command;


		protected:

			// -- protected members -------------------------------------------

			/* argv */
			std::vector<std::string> _argv;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			command(void) noexcept = default;

			/* argv constructor */
			command(std::vector<std::string>&& argv)
			: _argv{std::move(argv)} {
			}

			/* copy constructor */
			command(const self&) noexcept = default;

			/* move constructor */
			command(self&&) noexcept = default;

			/* destructor */
			virtual ~command(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public interface --------------------------------------------

			/* execute command */
			virtual void execute(sm::taskmaster&) const = 0;

	}; // class command

} // namespace sm

#endif // command_hpp
