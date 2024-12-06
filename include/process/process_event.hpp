#ifndef process_event_hpp
#define process_event_hpp

#include "io_event.hpp"
#include "system/unique_fd.hpp"
#include "process/process_id.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- P R O C E S S  E V E N T --------------------------------------------

	class process_event final : public sm::io_event {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process_event;


			// -- private members ---------------------------------------------

			/* unique fd */
			ft::unique_fd _fd;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_event(void) noexcept = default;

			/* pid constructor */
			process_event(const sm::process_id& pid)
			: _fd{pid.open()} {
			}

			/* deleted copy constructor */
			process_event(const self&) = delete;

			/* move constructor */
			process_event(self&&) noexcept = default;

			/* destructor */
			~process_event(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public override ---------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override {
				return static_cast<int>(_fd);
			}

			/* on event */
			auto on_event(const ::uint32_t& events) -> void override {

				//if (events & EPOLLIN) {
				//
				//}
			}

	}; // class process_event

} // namespace sm

#endif // process_event_hpp
