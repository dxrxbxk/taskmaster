#include "taskmaster/program_manager.hpp"



// -- public accessors --------------------------------------------------------

/* number of programs */
auto sm::program_manager::size(void) const noexcept -> std::size_t {
	return _programs.size();
}


// -- public modifiers --------------------------------------------------------

/* add program */
auto sm::program_manager::add_program(sm::unique_ptr<sm::program>&& program) -> void {

	// insert program by moving
	_programs.emplace(program->id(), std::move(program));
}


// -- public methods ----------------------------------------------------------

/* execute */
auto sm::program_manager::execute(sm::taskmaster& tm) -> void {

	// loop over programs
	for (auto& [id, program] : _programs) {

		// execute program
		program->execute(tm);
	}
}
