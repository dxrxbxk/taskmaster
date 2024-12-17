#ifndef program_hpp
#define program_hpp

#include "resources/unique_fd.hpp"
#include "containers/contiguous_cstr.hpp"
#include "system/fork.hpp"
#include "system/dup2.hpp"
#include "system/open.hpp"
#include "system/execve.hpp"
#include "system/access.hpp"

#include "core_affinity.hpp"
#include "events/monitor.hpp"

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

			/* stop */
			auto stop(void) -> void;


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

			auto numprocs(const sm::usize& value) noexcept -> void {
				_numprocs = value;
			}

			auto umask(const ::mode_t& value) noexcept -> void {
				_umask = value;
			}

			auto workingdir(std::string&& value) noexcept -> void {
				_workingdir = std::move(value);
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

			auto exitcodes(const int &value) noexcept -> void {
				_exitcodes.push_back(value);
			}

			auto startretries(const unsigned& value) noexcept -> void {
				_startretries = value;
			}

			auto starttime(const unsigned& value) noexcept -> void {
				_starttime = value;
			}

			auto stopsignal(const int& value) noexcept -> void {
				_stopsignal = value;
			}

			auto stoptime(const unsigned& value) noexcept -> void {
				_stoptime = value;
			}

			auto env_push(const char* arg) noexcept -> void {
				_env.push(arg);
			}


			// -- public accessors --------------------------------------------

			/* id */
			auto id(void) const noexcept -> const std::string& {
				return _id;
			}

			/* is running */
			auto is_running(void) const noexcept -> bool {
				return _pid != 0;
			}


			// -- public methods ----------------------------------------------

			/* info */
			auto info(void) const -> void {

				std::cout << "\x1b[32m------------------------- " << _id << "\x1b[0m" << "\r\n";;
				std::cout << "---------- pid: " << _pid << "\r\n";;
				std::cout << "---------- cmd: " << _cmd.data() << "\r\n";;
				std::cout << "----- numprocs: " << _numprocs << "\r\n";;
				std::cout << "-------- umask: " << std::oct << _umask << std::dec << "\r\n";;
				std::cout << "--- workingdir: " << _workingdir << "\r\n";;
				std::cout << "---- autostart: " << std::boolalpha << _autostart << "\r\n";;
				std::cout << "-- autorestart: " << _autorestart << "\r\n";;
				std::cout << std::noboolalpha;
				std::cout << "---- exitcodes: ";
				for (const auto& code : _exitcodes)
					std::cout << code << " ";
				std::cout << "\r\n";;
				std::cout << "- startretries: " << _startretries << "\r\n";;
				std::cout << "---- starttime: " << _starttime << "\r\n";;
				std::cout << "--- stopsignal: " << _stopsignal << "\r\n";;
				std::cout << "----- stoptime: " << _stoptime << "\r\n";;
				std::cout << "------- stdout: " << _stdout << "\r\n";;
				std::cout << "------- stderr: " << _stderr << "\r\n";;
				std::cout << "---------- env: " << _env.data() << "\r\n";;


			}


	}; // class program

} // namespace sm

#endif // program_hpp
