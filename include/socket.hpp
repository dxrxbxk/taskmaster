#ifndef socket_hpp
#define socket_hpp

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string>

#include "system/unique_fd.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {

	class addr;

	class socket : public ft::unique_fd {


		public:

			socket() noexcept = default;
			socket(int, int, int = 0);
			~socket() noexcept = default;
			socket(const socket &) = delete;
			socket(socket &&) noexcept = default;
			socket &operator=(const socket &) = delete;
			socket &operator=(socket &&) noexcept = default;
			operator int() const noexcept;

			void bind(const addr &) const;
			void listen(int = SOMAXCONN) const;
			socket accept(addr &) const;
			socket accept() const;

			void non_blocking() const;

			void shutdown() const;

			void reuse_address() const;

	};

	class addr {
		private:
			sockaddr_storage _addr;
			socklen_t _len;
		public:
			addr() noexcept;
			addr(const addr &) noexcept = default;
			addr(addr &&) noexcept = default;
			addr &operator=(const addr &) noexcept = default;
			addr &operator=(addr &&) noexcept = default;
			~addr() noexcept = default;

			addr(in_addr, in_port_t);
			addr(in6_addr, in_port_t);
			addr(const std::string &);


			const socklen_t &size() const noexcept;
			socklen_t &size() noexcept;

			const sockaddr_in &as_ipv4() const noexcept;
			const sockaddr_in6 &as_ipv6() const noexcept;
			const sockaddr &as_sockaddr() const noexcept;
			const sockaddr_un &as_local() const noexcept;

			sockaddr_in &as_ipv4() noexcept;
			sockaddr_in6 &as_ipv6() noexcept;
			sockaddr &as_sockaddr() noexcept;
			sockaddr_un &as_local() noexcept;


	}; // class addr

}


#endif // SOCKET_HPP
