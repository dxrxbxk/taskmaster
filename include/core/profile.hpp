#ifndef profile_hpp
#define profile_hpp

#include "containers/contiguous_cstr.hpp"
#include "types.hpp"

#include <string>
#include <vector>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O F I L E -------------------------------------------------------

	class profile final {


		// -- friends ---------------------------------------------------------

		/* parser as friend */
		friend class parser;


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::profile;


			// -- private members ---------------------------------------------

			/* id */
			std::string _id;

			/* command */
			sm::contiguous_cstr _cmd;

			/* number of processes */
			sm::usize _numprocs;

			/* umask */
			::mode_t _umask;

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

			/* working directory */
			std::string _workingdir;

			/* stdout redirect */
			std::string _stdout;

			/* stderr redirect */
			std::string _stderr;

			/* environment */
			sm::contiguous_cstr _env;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			profile(void) = delete;

			/* id constructor */
			profile(std::string&&);

			/* deleted copy constructor */
			profile(const profile&) = delete;

			/* deleted move constructor */
			profile(profile&&) = delete;

			/* destructor */
			~profile(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const profile&) -> profile& = delete;

			/* deleted move assignment operator */
			auto operator=(profile&&) -> profile& = delete;


			// -- public accessors --------------------------------------------

			/* id */
			auto id(void) const noexcept -> const std::string&;

			/* cmd */
			auto cmd(void) noexcept -> sm::contiguous_cstr&;

			/* const cmd */
			auto cmd(void) const noexcept -> const sm::contiguous_cstr&;

			/* numprocs */
			auto numprocs(void) const noexcept -> const sm::usize&;

			/* umask */
			auto umask(void) const noexcept -> const ::mode_t&;

			/* autostart */
			auto autostart(void) const noexcept -> const bool&;

			/* autorestart */
			auto autorestart(void) const noexcept -> const unsigned&;

			/* exitcodes */
			auto exitcodes(void) const noexcept -> const std::vector<int>&;

			/* startretries */
			auto startretries(void) const noexcept -> const unsigned&;

			/* starttime */
			auto starttime(void) const noexcept -> const unsigned&;

			/* stopsignal */
			auto stopsignal(void) const noexcept -> const int&;

			/* stoptime */
			auto stoptime(void) const noexcept -> const unsigned&;

			/* workingdir */
			auto workingdir(void) const noexcept -> const std::string&;

			/* stdout */
			auto stdout(void) const noexcept -> const std::string&;

			/* stderr */
			auto stderr(void) const noexcept -> const std::string&;

			/* env */
			auto env(void) noexcept -> sm::contiguous_cstr&;

			/* const env */
			auto env(void) const noexcept -> const sm::contiguous_cstr&;


			/* info */
			auto info(void) const -> void {

				std::cout << "\x1b[32m------------------------- " << _id << "\x1b[0m" << "\r\n";;
				std::cout << "---------- cmd: " << _cmd << "\r\n";;
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
				std::cout << "---------- env: " << _env << "\r\n";;

			}

	}; // class profile

} // namespace sm

#endif // profile_hpp
