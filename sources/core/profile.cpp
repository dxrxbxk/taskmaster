#include "core/profile.hpp"
#include "signal.hpp"
#include "environment.hpp"


// -- P R O F I L E -----------------------------------------------------------

// -- public lifecycle --------------------------------------------------------

/* id constructor */
sm::profile::profile(std::string&& id)
: _id{std::move(id)},
  _cmd{},
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
  _env{sm::environment::copy()} {
}


// -- public accessors --------------------------------------------------------

/* id */
auto sm::profile::id(void) const noexcept -> const std::string& {
	return _id;
}

/* cmd */
auto sm::profile::cmd(void) noexcept -> sm::contiguous_cstr& {
	return _cmd;
}

/* const cmd */
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
auto sm::profile::autorestart(void) const noexcept -> sm::profile::start_type {
	return  _autorestart;
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
auto sm::profile::env(void) noexcept -> sm::contiguous_cstr& {
	return _env;
}

/* const env */
auto sm::profile::env(void) const noexcept -> const sm::contiguous_cstr& {
	return _env;
}


// -- public methods ----------------------------------------------------------

/* hash */
auto sm::profile::hash(void) const noexcept -> sm::usize {

	sm::usize hash = 0U;

	// cmd
	hash += _cmd.hash();

	// numprocs
	hash = (hash ^ _numprocs) * 16777619U;

	// umask
	hash = (hash ^ _umask) * 16777619U;

	// autostart
	hash = (hash ^ _autostart) * 16777619U;

	// autorestart
	hash = (hash ^ static_cast<unsigned>(_autorestart)) * 16777619U;

	// exitcodes
	for (const auto& exitcode : _exitcodes)
		hash = (hash ^ static_cast<unsigned>(exitcode)) * 16777619U;

	// startretries
	hash = (hash ^ _startretries) * 16777619U;

	// starttime
	hash = (hash ^ _starttime) * 16777619U;

	// stopsignal
	hash = (hash ^ static_cast<unsigned>(_stopsignal)) * 16777619U;

	// stoptime
	hash = (hash ^ _stoptime) * 16777619U;

	// workingdir
	for (const auto& c : _workingdir)
		hash = (hash ^ static_cast<unsigned>(c)) * 16777619U;

	// stdout
	for (const auto& c : _stdout)
		hash = (hash ^ static_cast<unsigned>(c)) * 16777619U;

	// stderr
	for (const auto& c : _stderr)
		hash = (hash ^ static_cast<unsigned>(c)) * 16777619U;

	// env
	hash += _env.hash();


	return hash;
}
