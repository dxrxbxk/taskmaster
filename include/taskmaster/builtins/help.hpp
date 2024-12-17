#ifndef help_hpp
#define help_hpp

#include "taskmaster/builtins/command.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	// -- C M D  N A M E S P A C E --------------------------------------------

	namespace cmd {

		// -- H E L P ---------------------------------------------------------

		class help final : public sm::command {


			public:

				// -- public lifecycle ----------------------------------------

				/* deleted default constructor */
				help(void) = delete;

				/* argv constructor */
				help(std::vector<std::string>&& argv)
				: sm::command{std::move(argv)} {
				}


				// -- public overrides ----------------------------------------

				/* execute command */
				auto execute(sm::taskmaster&) const -> void override {
					for (const auto& arg : _argv)
						std::cout << arg << " : ";
					std::cout << std::endl;
				}

		}; // class help

	} // namespace cmd

} // namespace sm

#endif // help_hpp
