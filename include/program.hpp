#ifndef program2_hpp
#define program2_hpp

#include "resources/unique_fd.hpp"
#include "containers/contiguous_cstr.hpp"
#include "system/fork.hpp"
#include "system/dup2.hpp"
#include "system/open.hpp"
#include "system/execve.hpp"
#include "system/access.hpp"

#include "time/timer.hpp"
#include "events/monitor.hpp"

#include <string>
#include <vector>
#include <unistd.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program final : public sm::listener {


		public:

			class data;


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
			// The command to use to launch the program
			sm::contiguous_cstr _cmd;

			/* number of processes */
			// The number of processes to start and keep running
			sm::usize _numprocs;

			/* umask */
			// An umask to set before launching the program
			::mode_t _umask;

			/* auto start */
			// Whether to start this program at launch or not
			bool _autostart;
			
			/* auto restart */
			// Whether the program should be restarted always, never, or on unexpected exits only
			unsigned _autorestart;

			/* exit codes */
			// Which return codes represent an "expected" exit status
			std::vector<int> _exitcodes;

			/* start retries */
			// How many times a restart should be attempted before aborting
			unsigned _startretries;

			/* start time */
			// How long the program should be running after it’s started for it to be considered "successfully started"
			unsigned _starttime;

			/* stop signal */
			// Which signal should be used to stop (i.e. exit gracefully) the program
			int _stopsignal;

			/* stop time */
			// How long to wait after a graceful stop before killing the program
			unsigned _stoptime;

			/* working directory */
			// A working directory to set before launching the program
			std::string _workingdir;

			/* stdout redirect */
			// Options to discard the program’s stdout/stderr or to redirect them to files
			std::string _stdout;

			/* stderr redirect */
			// Options to discard the program’s stdout/stderr or to redirect them to files
			std::string _stderr;

			/* environment */
			// Environment variables to set before launching the program
			sm::contiguous_cstr _env;


			/* start timer */
			sm::timer _starttimer;

			/* is starting */
			bool _is_starting;

			/* stop timer */
			sm::timer _stoptimer;

			/* is stopping */
			bool _is_stopping;


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

			/* destructor */
			~program(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(sm::taskmaster&) -> void;

			/* stop */
			auto stop(sm::taskmaster&) -> void;

			/* start event */
			auto start_event(sm::taskmaster&) -> void;

			/* stop event */
			auto stop_event(sm::taskmaster&) -> void;


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

			/* pid */
			auto pid(void) const noexcept -> const ::pid_t& {
				return _pid;
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

			/* status */
			auto status(void) const -> void {
				if (_pid == 0) {

					sm::logger::info("program: ",
									 std::string_view{_cmd[0U]},
									 " \x1b[31mnot\x1b[0m running");

				}
				else {

					sm::logger::info("program: ",
									 std::string_view{_cmd[0U]},
									 " \x1b[32mrunning\x1b[0m");

				}
			}

	}; // class program





} // namespace sm

#endif // program_hpp
