#include "core/program.hpp"


// -- P R O G R A M -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
sm::program2::program2(sm::shared_ptr<sm::profile>&& profile)
: _profile{std::move(profile)},
  _processes{},
  _gid{0} {

	// create processes
	for (auto i = 0U; i < _profile->numprocs(); ++i)
		_processes.emplace_back(_profile);
}

/* destructor */
sm::program2::~program2(void) noexcept {
}


// -- public accessors --------------------------------------------------------

/* profile */
auto sm::program2::profile(void) noexcept -> sm::profile& {
	return *_profile;
}

/* processes */
auto sm::program2::processes(void) noexcept -> std::vector<sm::process>& {
	return _processes;
}

/* group id */
auto sm::program2::group_id(void) const noexcept -> const ::pid_t& {
	return _gid;
}


// -- public modifiers --------------------------------------------------------

/* group id */
auto sm::program2::group_id(const ::pid_t& gid) noexcept -> void {
	_gid = gid;
}
