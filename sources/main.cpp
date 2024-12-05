#include <unistd.h>
#include <iostream>

#include <vector>
#include <string>

#include <sys/wait.h>
#include "system/syscall.hpp"

#include "dispatch.hpp"


namespace sys {


	/* malloc */
	template <typename T>
	auto malloc(const ::size_t& size) -> T* {

		T* ptr = static_cast<T*>(::malloc(size * sizeof(T)));

		if (ptr == nullptr)
			throw std::bad_alloc();

		return ptr;
	}

	/* realloc */
	template <typename T>
	auto realloc(T* ptr, const ::size_t& size) -> T* {

		T* new_ptr = static_cast<T*>(::realloc(ptr, size * sizeof(T)));

		if (new_ptr == nullptr)
			throw std::bad_alloc();

		return new_ptr;
	}

	/* free */
	template <typename T>
	auto free(T* ptr) -> void {
		::free(ptr);
	}

	/* setsid */
	auto setsid(void) -> ::pid_t {

		const auto sid = ::setsid();

		if (sid == -1)
			throw std::runtime_error("setsid failed");

		return sid;
	}

} // namespace sys


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

	class contiguous_cstr final {

	};


	class exec_args final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tsk::exec_args;


			// -- private members ---------------------------------------------

			/* data */
			char** _data;

			/* size */
			::size_t _size;

			/* capacity */
			::size_t _capacity;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			exec_args(void) noexcept
			: _data{nullptr}, _size{0U}, _capacity{0U} {
			}


			/* copy constructor */
			exec_args(const self& other) {
			}

			/* move constructor */
			exec_args(self&& other) noexcept
			: _data{other._data}, _size{other._size}, _capacity{other._capacity} {

				// invalidate other
				other._init();
			}

			/* destructor */
			~exec_args(void) noexcept {

				if (_data == nullptr)
					return;

				for (::size_t i = 0U; i < _size; ++i)
					sys::free(_data[i]);

				sys::free(_data);
			}


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self& other) -> self& {
				return *this;
			}

			/* move assignment operator */
			auto operator=(self&& other) noexcept -> self& {

				// check for self assignment
				if (this == &other)
					return *this;

				// invalidate this
				self::_free();

					// move other
					_data = other._data;
					_size = other._size;
				_capacity = other._capacity;

				// invalidate other
				other._init();

				// done
				return *this;
			}


			// -- public modifiers --------------------------------------------

			/* push */
			auto push(const std::string& arg) noexcept -> void {

				// check if we need to allocate
				if (_size == _capacity) {

					// calculate new capacity
					const ::size_t cap = _capacity == 0U ? 1U : (_capacity * 2U);

					// reallocate
					_data = sys::realloc(_data, cap + 1U);

					// update capacity
					_capacity = cap;

					// null terminate
					_data[_capacity] = nullptr;
				}

				// allocate new string
				_data[_size] = sys::malloc<char>(arg.size() + 1U);

				// copy string
				std::copy(arg.begin(), arg.end(), _data[_size]);

				// null terminate
				_data[_size][arg.size()] = '\0';

				// increment size
				++_size;
			}


			// -- public accessors --------------------------------------------

			/* data */
			auto data(void) noexcept -> char** {
				return _data;
			}

			/* data */
			auto data(void) const noexcept -> const char* const* {
				return _data;
			}


		private:

			// -- private methods ---------------------------------------------

			/* init */
			auto _init(void) noexcept -> void {
					_data = nullptr;
					_size = 0U;
				_capacity = 0U;
			}

			/* free */
			auto _free(void) noexcept -> void {

				if (_data == nullptr)
					return;

				for (::size_t i = 0U; i < _size; ++i)
					sys::free(_data[i]);

				sys::free(_data);
			}

	};





}

#include <sys/syscall.h>
#include <errno.h>
#include <cstring>

#include "dispatch.hpp"
#include "process/wstatus.hpp"

#include "fcntl.h"


namespace ft {



	class process_id final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = ft::process_id;


			// -- private members ---------------------------------------------

			/* pid */
			::pid_t _pid;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			process_id(void) noexcept
			: _pid{0} {
			}

			/* pid constructor */
			process_id(const ::pid_t& pid) noexcept
			: _pid{pid} {
			}

			/* copy constructor */
			process_id(const self&) noexcept = default;

			/* move constructor */
			process_id(self&&) noexcept = default;

			/* destructor */
			~process_id(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public convertion operators ---------------------------------

			/* pid_t conversion operator */
			operator const ::pid_t&(void) const noexcept {
				return _pid;
			}


			// -- public methods ----------------------------------------------

			/* open */
			auto open(void) const -> ft::unique_fd {

				// open pidfd with syscall
				return ft::unique_fd{static_cast<int>(
							ft::syscall(SYS_pidfd_open, _pid, 0))
				};
			}

			/* wait */
			auto wait(const int& options = 0) const -> sm::wstatus {

				int status;

				const auto state = ::waitpid(_pid, &status, options);

				if (state == -1)
					throw std::runtime_error("waitpid failed");

				// WNOHANG
				// return immediately if no child has exited.

				// WUNTRACED
				// also return if a child has stopped (but not traced via ptrace(2)).
				// Status for traced children which have stopped is provided even if this option is not specified.

				// WCONTINUED (since Linux 2.6.10)
				// also return if a stopped child has been resumed by delivery of SIGCONT.

				return sm::wstatus{status};
			}

			/* wait child */
			auto wait_child(const int& options = 0) const -> void {

				::waitpid(-_pid, nullptr, options);
			}

			// < -1
			// meaning wait for any child process whose process group ID is equal to the absolute value of pid.

			// -1
			// meaning wait for any child process.

			// 0
			// meaning wait for any child process whose process group ID is equal to that of the calling process at the time of the call to waitpid().

			// > 0
			// meaning wait for the child whose process ID is equal to the value of pid.


			// waitpid():
			// on success, returns the process ID of the child whose state has changed;
			// if WNOHANG was specified and one or more child(ren) specified by pid exist, but
			// have not yet changed state, then 0 is returned.  On failure, -1 is returned.



	}; // class pid

} // namespace ft


class process_event final : public sys::io_event {


	private:

		/* unique fd */
		ft::unique_fd _fd;


	public:

		process_event(const ft::process_id& pid)
		: _fd{pid.open()} {
		}

		auto fd(void) const noexcept -> int override {
			return static_cast<int>(_fd);
		}

		auto on_event(const ::uint32_t& events) -> void override {

			if (events & EPOLLIN) {

			}
		}

};


class reader final : public sys::io_event {

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




#include <termios.h>

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

		sys::dispatch d;

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

	/*
	try {

		tsk::daemon daemon{};
		daemon.run();
	}

	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}
	*/


	//auto ret = syscall(SYS_pidfd_open, getpid(), 0);
	//
	//if (ret == -1)
	//	std::cerr << "error: " << std::strerror(errno) << std::endl;
	//else {
	//	std::cout << "pidfd: " << ret << std::endl;
	//	int c = ::close((int)ret);
	//	if (c == -1)
	//		std::cerr << "error: " << std::strerror(errno) << std::endl;
	//}

	//tsk::process_info info;
	//
	//info.print();
	//
	//return 0;

	return 0;
}
