#ifndef program_hpp
#define program_hpp

#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "common/resources/unique_ptr.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O G R A M -------------------------------------------------------

	class program final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program;


			// -- private constants -------------------------------------------

			enum : unsigned {
				restart_false      = 0U,
				restart_true       = 1U,
				restart_unexpected = 2U,
			};


			// -- private members ---------------------------------------------


			char** _cmd;

			unsigned _numprocs;

			std::string _umask;
			std::string _working_dir;

			bool _auto_start;
			
			unsigned _auto_restart;

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


	class program_manager final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::program_manager;


			// -- private members ---------------------------------------------
			using map_prog = std::unordered_map<std::string, sm::unique_ptr<sm::program>>;

			map_prog _programs;

			using map_id = std::unordered_map<pid_t, sm::program*>;

		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			program_manager(void) noexcept = default;

			/* copy constructor */
			program_manager(const self&) = default;

			/* move constructor */
			program_manager(self&&) noexcept = default;

			/* destructor */
			~program_manager(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------


	};

} // namespace sm

#endif // program_hpp
