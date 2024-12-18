#include "program_manager.hpp"



// -- public accessors --------------------------------------------------------

/* number of programs */
auto sm::program_manager::size(void) const noexcept -> std::size_t {
	return _programs.size();
}


// -- public modifiers --------------------------------------------------------

/* add program */
auto sm::program_manager::add_program(sm::unique_ptr<sm::program>&& program) -> void {

	//program->info();

	// insert program by moving
	_programs.emplace(program->id(), std::move(program));
}


// -- public methods ----------------------------------------------------------

/* start all */
auto sm::program_manager::start_all(sm::taskmaster& tm) -> void {

	// loop over programs
	for (auto& [id, program] : _programs) {

		// execute program
		program->start(tm);
	}
}
