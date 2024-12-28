#ifndef process_hpp
#define process_hpp

#include "time/timer.hpp"
#include "events/monitor.hpp"
#include "resources/shared_ptr.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* profile */
	class profile;

	/* program */
	class program;


	// -- P R O C E S S -------------------------------------------------------

	class process final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process;


			// -- private constants -------------------------------------------

			/* state type */
			enum state_type : unsigned {

				// The process has been stopped due to a stop request or has never been started.
				S_STOPPED,

				// The process is starting due to a start request.
				S_STARTING,

				// The process is running.
				S_RUNNING,

				// The process entered the STARTING state but subsequently exited too quickly
				// (before the time defined in startsecs) to move to the RUNNING state.
				S_BACKOFF,

				// The process is stopping due to a stop request.
				S_STOPPING,

				// The process exited from the RUNNING state (expectedly or unexpectedly).
				S_EXITED,

				// The process could not be started successfully.
				S_FATAL,

				// The process is in an unknown state (supervisord programming error).
				S_UNKNOWN,


				STATE_TYPE_MAX,
			};


			// -- private members ---------------------------------------------

			/* profile */
			sm::shared_ptr<sm::profile> _profile;

			/* state */
			state_type _state;

			/* pid */
			::pid_t _pid;

			/* pid descriptor */
			sm::unique_fd _pidfd;

			/* start timer */
			sm::timer _starttimer;

			/* stop timer */
			sm::timer _stoptimer;

			/* is retarting */
			bool _is_restarting;

			/* _restart_try */
			sm::usize _restart_try;


		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			process(void) = delete;

			/* profile constructor */
			process(const sm::shared_ptr<sm::profile>&);

			/* deleted copy constructor */
			process(const self&) = delete;

			/* move constructor */
			process(self&&) noexcept;

			/* destructor */
			~process(void) noexcept;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on_event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;


			// -- public methods ----------------------------------------------

			/* start */
			auto start(sm::monitor&) -> void;

			/* stop */
			auto stop(sm::taskmaster&) -> void;

			/* restart */
			auto restart(sm::taskmaster&) -> void;

			/* disconnect */
			auto disconnect(sm::taskmaster&) -> void;

			/* status */
			auto status(void) const -> void;


			// -- public callbacks --------------------------------------------

			/* start event */
			auto start_event(sm::taskmaster&) -> void;

			/* stop event */
			auto stop_event(sm::taskmaster&) -> void;


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const noexcept -> bool;

			/* pid */
			auto pid(void) const noexcept -> const ::pid_t&;





			enum command_type : unsigned {

				// user send a start request
				C_START,

				// user send a stop request
				C_STOP,

				// user send a restart request
				C_RESTART,

				// maximum number of command types
				ACTION_TYPE_MAX,
			};

	}; // class process

} // namespace sm

#endif // process_hpp
