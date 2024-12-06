#include <unistd.h>
#include <iostream>

#include <vector>
#include <string>

#include <termios.h>
#include <sys/wait.h>
#include "system/syscall.hpp"

#include "dispatch.hpp"

#include "memory/malloc.hpp"

#include <sys/syscall.h>
#include <errno.h>
#include <cstring>

#include "dispatch.hpp"
#include "process/wstatus.hpp"
#include "process/process_id.hpp"

#include "fcntl.h"
#include "core/daemon.hpp"



namespace tsk {


	class process_info final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tsk::process_info;


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

	};

}




class reader final : public sm::io_event {

	private:


	public:

		reader(void) noexcept = default;


		auto fd(void) const noexcept -> int override {
			return STDIN_FILENO;
		}

		auto on_event(const ::uint32_t& events) -> void override {

			if (events & EPOLLIN) {

				char buffer[1024];
				const auto bytes = ::read(STDIN_FILENO, buffer, sizeof(buffer));

				if (bytes == -1)
					throw std::runtime_error("read failed");

				buffer[bytes] = '\0';

				std::cout << "read: " << buffer << std::endl;
			}
		}
};





auto main(void) -> int {

	try {


		sm::wstatus status{};



		// get terminal attributes
		struct termios term;
		if (::tcgetattr(STDIN_FILENO, &term) == -1)
			throw std::runtime_error("tcgetattr failed");

		struct termios raw = term;

		// disable canonical mode
		raw.c_lflag &= static_cast<::tcflag_t>(~ICANON);

		// disable echo
		raw.c_lflag &= static_cast<::tcflag_t>(~ECHO);

		// set new attributes
		if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
			throw std::runtime_error("tcsetattr failed");


		reader r{};

		sm::dispatch d;

		d.add(r, EPOLLIN);

		d.wait();

		d.del(r);


		// restore terminal attributes
		if (::tcsetattr(STDIN_FILENO, TCSAFLUSH, &term) == -1)
			throw std::runtime_error("tcsetattr failed");
	}


	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}

	return 0;

	try {

		sm::daemon daemon{};
		daemon.run();
	}

	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}

	//tsk::process_info info;
	//
	//info.print();
	//
	//return 0;

	return 0;
}
