#ifndef program_hpp
#define program_hpp

#include <map>
#include <string>
#include <vector>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program;


			// -- private members ---------------------------------------------


			char** _cmd;

			unsigned _numprocs;

			std::string _umask;
			std::string _working_dir;

			bool _auto_start;
			
			bool _auto_restart;

			std::vector<int> _exit_codes;

			unsigned _start_retries;

			unsigned _start_time;

			int _stop_signal;

			unsigned _stop_time;

			std::string _stdout;
			std::string _stderr;

			std::map<std::string, std::string> _env;


		public:

			// -- public modifiers --------------------------------------------

			/* auto start */
			auto auto_start(const bool& value) noexcept -> void {
				_auto_start = value;
			}

			/* auto restart */
			auto auto_restart(const bool& value) noexcept -> void {
				_auto_restart = value;
			}

	}; // class program

} // namespace sm

#endif // program_hpp
