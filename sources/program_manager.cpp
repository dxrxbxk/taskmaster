#include "program_manager.hpp"



// -- public accessors --------------------------------------------------------

/* number of programs */
auto sm::program_manager::size(void) const noexcept -> std::size_t {
	return _programs.size();
}


// -- public modifiers --------------------------------------------------------

/* add profile */
auto sm::program_manager::add_profile(sm::shared_ptr<sm::profile>&& profile) -> void {

	//profile->info();

	// insert program by moving
	_programs.emplace(profile->id(), std::move(profile));
}

/* autostart */
auto sm::program_manager::autostart(sm::monitor& monitor) -> void {

	// loop over programs
	for (auto& [id, program] : _programs) {

		// execute program
		program.autostart(monitor);
	}
}


// -- public methods ----------------------------------------------------------

/* start all */
auto sm::program_manager::start_all(sm::taskmaster& tm) -> void {

	// loop over programs
	for (auto& [id, program] : _programs) {

		// execute program
		//program->start(tm);
	}
}

/* hot swap */
auto sm::program_manager::hot_swap(sm::monitor& monitor, self&& other) -> void {


	std::vector<std::string> removed;

	// erase removed ids
	for (auto& [id, _] : _programs) {
		if (other._programs.contains(id) == false)
			removed.push_back(id);
	}

	// erase removed programs
	for (const auto& id : removed) {
		sm::logger::hint("removing program: ", id);
		_programs.erase(id);
	}

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
