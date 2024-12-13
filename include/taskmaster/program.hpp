#ifndef program_hpp
#define program_hpp

#include "common/resources/unique_fd.hpp"
#include "common/contiguous_cstr.hpp"
#include "common/system/fork.hpp"
#include "common/system/dup2.hpp"
#include "common/system/open.hpp"
#include "common/system/execve.hpp"
#include "common/system/access.hpp"

#include "taskmaster/core_affinity.hpp"
#include "taskmaster/events/monitor.hpp"

#include <string>
#include <vector>
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program;


			// -- private constants -------------------------------------------

			enum : unsigned {
				FALSE      = 0U,
				TRUE       = 1U,
				UNEXPECTED = 2U,
			};


			// -- private members ---------------------------------------------

			/* id */
			std::string _id;

			/* pid */
			::pid_t _pid;

			/* pid descriptor */
			sm::unique_fd _pidfd;

			/* command */
			sm::contiguous_cstr _cmd;

			/* number of processes */
			sm::usize _numprocs;

			/* umask */
			::mode_t _umask;

			/* working directory */
			std::string _workingdir;

			/* auto start */
			bool _autostart;
			
			/* auto restart */
			unsigned _autorestart;

			/* exit codes */
			std::vector<int> _exitcodes;

			/* start retries */
			unsigned _startretries;

			/* start time */
			unsigned _starttime;

			/* stop signal */
			int _stopsignal;

			/* stop time */
			unsigned _stoptime;

			/* stdout redirect */
			std::string _stdout;

			/* stderr redirect */
			std::string _stderr;

			/* environment */
			sm::contiguous_cstr _env;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			program(void) = delete;

			/* id constructor */
			program(std::string&&);

			/* deleted copy constructor */
			program(const self&) = delete;

			/* deleted move constructor */
			program(self&&) = delete;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* execute */
			auto execute(sm::taskmaster&) -> void;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on_event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;


			auto disconnect(sm::taskmaster&) -> void;


			// -- public modifiers --------------------------------------------

			/* cmd push */
			auto cmd_push(const char* arg) noexcept -> void {
				_cmd.push(arg);
			}

			/* auto start */
			auto autostart(const bool& value) noexcept -> void {
				_autostart = value;
			}

			/* auto restart */
			auto autorestart(const bool& value) noexcept -> void {
				_autorestart = value;
			}

			/* stdout */
			auto stdout(std::string&& value) noexcept -> void {
				_stdout = std::move(value);
			}

			/* stderr */
			auto stderr(std::string&& value) noexcept -> void {
				_stderr = std::move(value);
			}


			// -- public accessors --------------------------------------------

			/* id */
			auto id(void) const noexcept -> const std::string& {
				return _id;
			}

	}; // class program

} // namespace sm

#endif // program_hpp
