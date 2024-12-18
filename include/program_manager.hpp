#ifndef program_manager_hpp
#define program_manager_hpp

#include "resources/unique_ptr.hpp"
#include "program.hpp"

#include <unordered_map>
#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M  M A N A G E R ----------------------------------------

	class program_manager final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program_manager;

			using map_prog = std::unordered_map<std::string, sm::unique_ptr<sm::program>>;
			using map_id = std::unordered_map<pid_t, sm::program*>;


			// -- private members ---------------------------------------------

			/* map of programs */
			map_prog _programs;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			program_manager(void) = default;

			/* deleted copy constructor */
			program_manager(const self&) = delete;

			/* deleted move constructor */
			program_manager(self&&) = delete;

			/* destructor */
			~program_manager(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public modifiers --------------------------------------------

			/* add program */
			auto add_program(sm::unique_ptr<sm::program>&&) -> void;


			// -- public accessors --------------------------------------------

			/* number of programs */
			auto size(void) const noexcept -> std::size_t;

			/* has program */
			auto has_program(const std::string& id) const noexcept -> bool {
				return _programs.find(id) != _programs.end();
			}

			/* get program */
			auto get_program(const std::string& id) -> sm::program& {

				auto it = _programs.find(id);

				return *it->second;
			}


			// -- public methods ----------------------------------------------

			/* start all */
			auto start_all(sm::taskmaster&) -> void;

			/* info */
			auto info(void) const -> void {
				for (const auto& [id, program] : _programs) {
					program->info();
				}
			}

	}; // class program_manager

} // namespace sm

#endif // program_manager_hpp
