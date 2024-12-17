#ifndef list_hpp
#define list_hpp

#include "taskmaster/builtins/command.hpp"
// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	// -- C M D  N A M E S P A C E --------------------------------------------

	namespace cmd {


		// -- L I S T ---------------------------------------------------------

		class list final : public sm::command {

			public:

				// -- public lifecycle ----------------------------------------

				/* deleted default constructor */
				list(void) = delete;

				/* argv constructor */
				list(std::vector<std::string>&& argv)
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
#endif // list_hpp
