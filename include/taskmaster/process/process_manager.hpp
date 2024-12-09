#ifndef process_manager_hpp
#define process_manager_hpp

#include <vector>
#include "taskmaster/process/process_event.hpp"


namespace sm {


	class process_manager final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process_manager;


			// -- private members ---------------------------------------------

			/* processes */
			std::vector<sm::process> _processes;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_manager(void) = default;


			// -- public modifiers --------------------------------------------

			/* add */
			auto add(const sm::process_id& id) -> sm::process& {
				_processes.emplace_back(id);
				return _processes.back();
			}

			/* add */
			auto add(sm::process&& pro) -> sm::process& {
				_processes.push_back(static_cast<sm::process&&>(pro));
				return _processes.back();
			}



	}; // class process_manager

} // namespace sm

#endif // process_manager_hpp
