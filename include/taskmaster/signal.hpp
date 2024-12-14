#ifndef signal_hpp
#define signal_hpp

#include "taskmaster/events/monitor.hpp"
#include "common/resources/unique_fd.hpp"

#include <unordered_map>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- S I G N A L ---------------------------------------------------------

	class signal final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::signal;

			using map_type = std::unordered_map<std::string, int>;


			// -- private members ---------------------------------------------

			/* pipe */
			sm::unique_fd _pipe[2U];

			map_type _signals;


			// -- private static methods --------------------------------------

			/* signal handler */
			static void _handler(const int) noexcept;

			/* record */
			static auto _record(const int&) -> void;

			/* record */
			template <int... sigs>
			static auto _record(void) -> void {
				(self::_record(sigs), ...);
			}

			// -- private methods ---------------------------------------------
			
			void _fill_map(void);

			// -- private lifecycle -------------------------------------------

			/* default constructor */
			signal(void);

			/* deleted copy constructor */
			signal(const self&) = delete;

			/* deleted move constructor */
			signal(self&&) = delete;

			/* destructor */
			~signal(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


		public:

			// -- public static methods ---------------------------------------

			/* shared */
			static auto shared(void) -> self&;

			static auto to_int(const std::string& name) -> int;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;


	}; // class signal

} // namespace sm

#endif // signal_hpp
