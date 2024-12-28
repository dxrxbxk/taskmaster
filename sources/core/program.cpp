#include "core/program.hpp"


// -- P R O G R A M -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
sm::program::program(sm::shared_ptr<sm::profile>&& profile)
: _profile{std::move(profile)},
  _processes{},
  _hash{_profile->hash()} {

	// create processes
	for (auto i = 0U; i < _profile->numprocs(); ++i)
		_processes.emplace_back(_profile);
}


// -- public accessors --------------------------------------------------------

/* profile */
auto sm::program::profile(void) noexcept -> sm::profile& {
	return *_profile;
}

/* processes */
auto sm::program::processes(void) noexcept -> std::vector<sm::process>& {
	return _processes;
}


/* status */
auto sm::program::status(void) const -> void {

	// loop over processes
	for (auto& process : _processes) {

		// get process status
		process.status();
	}
}


// -- public modifiers --------------------------------------------------------

/* autostart */
auto sm::program::autostart(sm::monitor& monitor) -> void {

	// start processes
	for (auto& process : _processes) {

		if (_profile->autostart())
			process.start(monitor);
	}
}


// -- public methods ----------------------------------------------------------

/* hot swap */
auto sm::program::hot_swap(sm::monitor& monitor, self&& other) -> void {

	if (_profile->hash() != other._profile->hash()) {
		*this = std::move(other);
		sm::logger::hint("program: ", _profile->id(), " hot swapped");

		self::autostart(monitor);
	}
}

			/*
			   condition to replace program:
			   different id
			   numprocs changed (only)

			   cmd yes
			   umask yes
			   stdout yes
			   stderr yes
			   env yes
			   workingdir yes

			   exitcodes ?
			   autorestart ?

			   startretries nope !
			   starttime nope !
			   stopsignal nope !
			   stoptime nope !
			*/

