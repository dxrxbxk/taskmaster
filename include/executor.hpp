#ifndef executor_hpp
#define executor_hpp

#include <string>
#include <vector>
#include <unordered_map>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* taskmaster */
	class taskmaster;


	// -- E X E C U T O R -----------------------------------------------------

	class executor final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::executor;

			/* argv type */
			using argv_type = std::vector<std::string>;

			/* function type */
			using functor = auto (self::*)(sm::taskmaster&, const argv_type&) -> void;


			// -- private members ---------------------------------------------

			/* map of commands */
			std::unordered_map<std::string, functor> _functions;


			// -- private methods ---------------------------------------------

			/* stop */
			auto _stop(sm::taskmaster&,  const argv_type&) -> void;

			/* list */
			auto _list(sm::taskmaster&,  const argv_type&) -> void;

			/* start */
			auto _start(sm::taskmaster&, const argv_type&) -> void;

			/* help */
			auto _help(sm::taskmaster&,  const argv_type&) -> void;

			/* exit */
			auto _exit(sm::taskmaster&,  const argv_type&) -> void;

			/* fg */
			auto _fg(sm::taskmaster&,  const argv_type&) -> void;

			/* clear */
			auto _clear(sm::taskmaster&,  const argv_type&) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			executor(void)
			: _functions{} {

				_functions["stop" ] = &self::_stop;
				_functions["list" ] = &self::_list;
				_functions["start"] = &self::_start;
				_functions["help" ] = &self::_help;
				_functions["exit" ] = &self::_exit;
				_functions["fg"   ] = &self::_fg;
				_functions["clear"] = &self::_clear;

			}


			// -- public methods ----------------------------------------------

			/* execute */
			auto execute(sm::taskmaster&, const argv_type&) -> void;


	}; // class executor

} // namespace sm

#endif // executor_hpp
