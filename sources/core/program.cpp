#include "core/program.hpp"


// -- P R O G R A M -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
sm::program2::program2(std::string&& id, sm::shared_ptr<sm::profile>&& profile)
: _id{std::move(id)},
  _profile{std::move(profile)},
  _processes{} {

	// create processes
	
}

/* destructor */
sm::program2::~program2(void) noexcept {
}


// -- public accessors --------------------------------------------------------

/* profile */
auto sm::program2::profile(void) noexcept -> sm::profile& {
	return *_profile;
}
