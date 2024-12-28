#ifndef program_manager_hpp
#define program_manager_hpp

#include "resources/shared_ptr.hpp"
#include "core/program.hpp"

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

			using map_prog = std::unordered_map<std::string,
												sm::program>;



			// -- private members ---------------------------------------------

			/* map of programs */
			map_prog _programs;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			program_manager(void) = default;

			/* deleted copy constructor */
			program_manager(const self&) = delete;

			/* move constructor */
			program_manager(self&&) noexcept = default;

			/* destructor */
			~program_manager(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* add profile */
			auto add_profile(sm::shared_ptr<sm::profile>&&) -> void;

			/* autostart */
			auto autostart(sm::monitor&) -> void;


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

				return it->second;
			}

			/* status */
			auto status(void) const -> void {
				for (const auto& [id, program] : _programs)
					program.status();
			}


			// -- public methods ----------------------------------------------

			/* start all */
			auto start_all(sm::taskmaster&) -> void;

			/* info */
			auto info(void) const -> void {
				//for (const auto& [id, program] : _programs) {
				//	program.info();
				//}
			}



			/* hot swap */
			auto hot_swap(sm::monitor& monitor, self&& other) -> void {

				// loop over other programs
				for (auto& [id, program] : other._programs) {

					auto it = _programs.find(id);

					// new program
					if (it == _programs.end()) {
						// add program
						auto pair = _programs.insert({id, std::move(program)});
						// launch program
						pair.first->second.autostart(monitor);
					}
					// hot swap program
					else {
						it->second.hot_swap(monitor, std::move(program));
					}
				}
			}



	}; // class program_manager

} // namespace sm

#endif // program_manager_hpp
