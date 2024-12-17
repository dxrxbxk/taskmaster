#ifndef stop_hpp
#define stop_hpp

#include "taskmaster/builtins/command.hpp"
// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	// -- C M D  N A M E S P A C E --------------------------------------------

	namespace cmd {

		// -- S T O P ---------------------------------------------------------

		class stop final : public sm::command {

			public:

				// -- public lifecycle ----------------------------------------

				/* deleted default constructor */
				stop(void) = delete;

				/* argv constructor */
				stop(std::vector<std::string>&& argv)
				: sm::command{std::move(argv)} {
				}

				// -- public overrides ----------------------------------------

				/* execute command */
				auto execute(sm::taskmaster&) const -> void override {

					for (const auto& arg : _argv)
						std::cout << arg << std::endl;
				}

		}; // class help

	} // namespace cmd

} // namespace sm


#endif // stop_hpp
