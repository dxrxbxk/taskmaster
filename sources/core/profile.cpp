#include "core/profile.hpp"
#include "signal.hpp"


// -- P R O F I L E -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* default constructor */
sm::profile::profile(void)
: _cmd{},
  _numprocs{1U},
  _umask{0777U},
  _autostart{false},
  _autorestart{false},
  _exitcodes{},
  _startretries{0U},
  _starttime{1U},
  _stopsignal{SIGTERM},
  _stoptime{1U},
  _workingdir{"."},
  _stdout{"/dev/null"},
  _stderr{"/dev/null"},
  _env{} {
}


// -- public accessors --------------------------------------------------------

/* cmd */
auto sm::profile::cmd(void) const noexcept -> const sm::contiguous_cstr& {
	return _cmd;
}

/* numprocs */
auto sm::profile::numprocs(void) const noexcept -> const sm::usize& {
	return _numprocs;
}

/* umask */
auto sm::profile::umask(void) const noexcept -> const ::mode_t& {
	return _umask;
}

/* autostart */
auto sm::profile::autostart(void) const noexcept -> const bool& {
	return _autostart;
}

/* autorestart */
auto sm::profile::autorestart(void) const noexcept -> const unsigned& {
	return _autorestart;
}

/* exitcodes */
auto sm::profile::exitcodes(void) const noexcept -> const std::vector<int>& {
	return _exitcodes;
}

/* startretries */
auto sm::profile::startretries(void) const noexcept -> const unsigned& {
	return _startretries;
}

/* starttime */
auto sm::profile::starttime(void) const noexcept -> const unsigned& {
	return _starttime;
}

/* stopsignal */
auto sm::profile::stopsignal(void) const noexcept -> const int& {
	return _stopsignal;
}

/* stoptime */
auto sm::profile::stoptime(void) const noexcept -> const unsigned& {
	return _stoptime;
}

/* workingdir */
auto sm::profile::workingdir(void) const noexcept -> const std::string& {
	return _workingdir;
}

/* stdout */
auto sm::profile::stdout(void) const noexcept -> const std::string& {
	return _stdout;
}

/* stderr */
auto sm::profile::stderr(void) const noexcept -> const std::string& {
	return _stderr;
}

/* env */
auto sm::profile::env(void) const noexcept -> const sm::contiguous_cstr& {
	return _env;
}
