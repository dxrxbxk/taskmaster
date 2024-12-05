#include <unistd.h>
#include <iostream>

#include <vector>
#include <string>

#include <sys/wait.h>

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

	/* fork */
	auto fork(void) -> ::pid_t {

		const ::pid_t pid = ::fork();

		if (pid == -1)
			throw std::runtime_error("fork failed");

		return pid;
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


	class pipe final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tsk::pipe;


			// -- private members ---------------------------------------------

			/* file descriptors */
			int _fds[2U];


			// -- private constants -------------------------------------------

			enum : unsigned int {
				RD = 0U,
				WR = 1U
			};


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			pipe(void) {

				// create pipe
				if (::pipe(_fds) == -1)
					throw std::runtime_error("pipe failed");
			}

			/* deleted copy constructor */
			pipe(const self&) = delete;

			/* deleted move constructor */
			pipe(self&&) = delete;

			/* destructor */
			~pipe(void) noexcept {

				// close read end
				if (_fds[RD] != -1)
					::close(_fds[RD]);

				// close write end
				if (_fds[WR] != -1)
					::close(_fds[WR]);
			}


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public modifiers --------------------------------------------

			/* close read end */
			auto close_read(void) noexcept -> void {

				// check descriptor
				if (_fds[RD] == -1)
					return;

				// close read end
				::close(_fds[RD]);
				_fds[RD] = -1;
			}

			/* close write end */
			auto close_write(void) noexcept -> void {

				// check descriptor
				if (_fds[WR] == -1)
					return;

				// close write end
				::close(_fds[WR]);
				_fds[WR] = -1;
			}


			// -- public methods ----------------------------------------------

			/* read */
			template <typename T>
			auto read(T& data) -> void {
				const auto bytes = ::read(_fds[RD], &data, sizeof(T));

				if (bytes == -1)
					throw std::runtime_error("read failed");
			}

			/* write */
			template <typename T>
			auto write(const T& data) -> void {
				const auto bytes = ::write(_fds[WR], &data, sizeof(T));

				if (bytes == -1)
					throw std::runtime_error("write failed");
			}

	};


	class daemon final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = tsk::daemon;


			// -- private classes ---------------------------------------------

			/* initializer */
			class initializer final {


				private:

					// -- private types ---------------------------------------

					/* self type */
					using self = tsk::daemon::initializer;


					// -- private lifecycle -----------------------------------

					/* default constructor */
					initializer(void) {

						// check if we are root
						if (::geteuid() != 0)
							throw std::runtime_error("taskmaster must be run as root");
					}

					/* deleted copy constructor */
					initializer(const self&) = delete;

					/* deleted move constructor */
					initializer(self&&) = delete;

					/* destructor */
					~initializer(void) noexcept = default;


					// -- private assignment operators ------------------------

					/* deleted copy assignment operator */
					auto operator=(const self&) -> self& = delete;

					/* deleted move assignment operator */
					auto operator=(self&&) -> self& = delete;


				public:

					// -- public static methods -------------------------------

					/* shared */
					static auto shared(void) -> self& {
						static self instance;
						return instance;
					}

			}; // class initializer


			// -- private members ---------------------------------------------

			/* process info */
			tsk::process_info _info;

			/* arguments */
			tsk::exec_args _args;



		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			daemon(void)
			: _info{nullptr}, _args{} {

				// initialize
				initializer::shared();
			}

			/* command constructor */
			daemon(const std::vector<std::string>& args)
			: _args{} {

				// initialize
				initializer::shared();

				for (const auto& arg : args)
					_args.push(arg);
			}

			/* copy constructor */
			daemon(const self&) = default;

			/* move constructor */
			daemon(self&&) noexcept = default;

			/* destructor */
			~daemon(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;



			// -- public methods ----------------------------------------------

			/* run */
			auto run(void) -> void {

				tsk::pipe pipe;
				auto pid = sys::fork();

				if (pid > 0) {

					// -- parent ----------------------------------------------

					// close write end
					pipe.close_write();

					::pid_t daemon_pid = 0;

					pipe.read(daemon_pid);
					pipe.close_read();

					::waitpid(pid, nullptr, 0);

					std::cout << "daemon pid: " << daemon_pid << std::endl;
				}
				else {

					// -- intermediate ----------------------------------------

					// new session
					sys::setsid();

					// close read end
					pipe.close_read();

					pid = sys::fork();

					if (pid > 0) {

						// -- second parent -----------------------------------

						// write daemon pid
						pipe.write(pid);
						pipe.close_write();

						// exit
						return;

					}
					else {

						// -- child -------------------------------------------

						// close write end
						pipe.close_write();
					}

				}

			}


			// -- public accessors --------------------------------------------

			/* is running */
			auto is_running(void) const -> bool {
				// done
				return true;
			}


	};


}

#include <sys/syscall.h>
#include <errno.h>
#include <cstring>

auto main(void) -> int {

	/*
	try {

		tsk::daemon daemon{};
		daemon.run();
	}

	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
	}
	*/


	auto ret = syscall(SYS_pidfd_open, getpid(), 0);

	if (ret == -1)
		std::cerr << "error: " << std::strerror(errno) << std::endl;
	else {
		std::cout << "pidfd: " << ret << std::endl;
		int c = ::close((int)ret);
		if (c == -1)
			std::cerr << "error: " << std::strerror(errno) << std::endl;
	}

	//tsk::process_info info;
	//
	//info.print();
	//
	//return 0;

	return 0;
}
