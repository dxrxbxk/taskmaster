#ifndef start_hpp
#define start_hpp

#include "taskmaster/builtins/command.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C M D  N A M E S P A C E --------------------------------------------

	namespace cmd {

		// -- S T A R T -------------------------------------------------------

		class start final : public sm::command {


			public:

				// -- public lifecycle ----------------------------------------

				/* deleted default constructor */
				start(void) = delete;

				/* argv constructor */
				start(std::vector<std::string>&& argv)
				: sm::command{std::move(argv)} {
				}

				// -- public overrides ----------------------------------------

				/* execute command */
				auto execute(sm::taskmaster&) const -> void override {

					for (const auto& arg : _argv)
						std::cout << arg << " : ";
					std::cout << std::endl;
					// argv 0 'start'
					// argv 1 'program'
				}

		}; // class help

	} // namespace cmd

} // namespace sm

#endif // start_hpp
