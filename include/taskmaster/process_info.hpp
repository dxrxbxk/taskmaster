#ifndef process_info_hpp
#define process_info_hpp

#include <unistd.h>
#include <iostream>

namespace sm {


	class process_info final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::process_info;


			// -- private members ---------------------------------------------

			/* process id */
			::pid_t _pid;

			/* process group id */
			::pid_t _pgid;

			/* session id */
			::pid_t _sid;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_info(void)
			: _pid{::getpid()}, _pgid{::getpgid(0)}, _sid{::getsid(0)} {
			}

			/* null constructor */
			process_info(decltype(nullptr))
			: _pid{0}, _pgid{0}, _sid{0} {
			}

			/* pid constructor */
			process_info(const ::pid_t& pid)
			: _pid{pid}, _pgid{::getpgid(pid)}, _sid{::getsid(pid)} {
			}

			/* copy constructor */
			process_info(const self&) noexcept = default;

			/* move constructor */
			process_info(self&&) noexcept = default;

			/* destructor */
			~process_info(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* print */
			auto print(void) const -> void {

				std::cout << "       process id: " << _pid << std::endl;
				std::cout << " process group id: " << _pgid << std::endl;
				std::cout << "       session id: " << _sid << std::endl;

				std::cout << "is session leader: " << is_session_leader() << std::endl;
				std::cout << "  is group leader: " << is_group_leader() << std::endl;

			}


			// -- public accessors --------------------------------------------

			/* is session leader */
			auto is_session_leader(void) const noexcept -> bool {
				return _sid == _pid;
			}

			/* is group leader */
			auto is_group_leader(void) const noexcept -> bool {
				return _pgid == _pid;
			}

			/* pid */
			auto process_id(void) const noexcept -> const ::pid_t& {
				return _pid;
			}

			/* pgid */
			auto group_id(void) const noexcept -> const ::pid_t& {
				return _pgid;
			}

			/* sid */
			auto session_id(void) const noexcept -> const ::pid_t& {
				return _sid;
			}

	}; // class process_info

} // namespace sm

#endif // process_info_hpp
