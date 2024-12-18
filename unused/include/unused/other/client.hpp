#ifndef client_hpp
#define client_hpp

#include "diagnostics/exception.hpp"
#include "network/socket.hpp"
#include "network/addr.hpp"
#include "readline.hpp"

//keepalive
#include <netinet/tcp.h>

#include <signal.h>



// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C L I E N T ---------------------------------------------------------

	class client final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::client;


			// -- private members ---------------------------------------------

			/* socket */
			sm::socket _socket;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client(void) noexcept = default;

			/* ip and port constructor */
			client(const ::in_addr& ip, const ::in_port_t& port)
			: _socket{AF_INET, SOCK_STREAM} {

				// connect the socket
				_socket.connect(sm::addr{ip, port});

				// setup timeout (5 seconds)
				//_socket.timeout(2'000, SO_RCVTIMEO);

				signal(SIGPIPE, SIG_IGN);


			}


			// -- public methods ----------------------------------------------

			/* run */
			auto run(void) -> void {

				sm::readline2 rl;

				// read from stdin
				while (true) {

					rl.read();

					if (rl.line() == "quit" || rl.line() == "exit")
						break;

					std::string msg{rl.line()};
					msg.append("\r\n");

					if (::send(_socket, msg.data(), msg.size(), 0) != static_cast<ssize_t>(msg.size())) {
						throw sm::system_error("send");
					}

					char buffer[1024U];
					const auto bytes = ::recv(_socket, buffer, sizeof(buffer), 0);

					if (bytes == -1) {
						throw sm::system_error("recv");
					}
					if (bytes == 0) {
						std::cout << "server closed connection\n";
						break;
					}
					::write(STDOUT_FILENO, buffer, static_cast<size_t>(bytes));
					::write(STDOUT_FILENO, "\n", 1U);
					continue;


					/*
					fd_set writefds;
					fd_set readfds;
					fd_set exceptfds;

					FD_ZERO(&writefds);
					FD_ZERO(&readfds);
					FD_ZERO(&exceptfds);

					FD_SET(_socket, &writefds);
					FD_SET(_socket, &readfds);
					FD_SET(_socket, &exceptfds);

					struct timeval tv;
					tv.tv_sec = 0;
					tv.tv_usec = 0;

					int ret = select(_socket + 1, &readfds, &writefds, &exceptfds, &tv);

					if (ret == -1) {
						throw sm::system_error("select");
					}


					// read event
					if (FD_ISSET(_socket, &readfds)) {
						std::cout << "socket is ready for reading\n";

						char buffer[1024U];
						const auto bytes = ::recv(_socket, buffer, sizeof(buffer), 0);

						if (bytes == -1) {
							throw sm::system_error("recv");
						}
						if (bytes == 0) {
							std::cout << "server closed connection\n";
							break;
						}
					}

					if (FD_ISSET(_socket, &writefds)) {
						std::cout << "socket is ready for writing\n";

					}

					if (FD_ISSET(_socket, &exceptfds)) {
						throw sm::system_error("socket exception");
					}
					*/




					//continue; // CCONTINUUUUUUUUUUUUUUU
					//
					//char buffer[1024U];
					//
					//unsigned attempts = 0U;
					//
					//for (unsigned i = 0U; i < 3U; ++i) {
					//
					//	const auto bytes = ::recv(_socket, buffer, sizeof(buffer), 0);
					//
					//	if (bytes == -1) {
					//		//if (errno == EAGAIN || errno == EWOULDBLOCK) {
					//		//	::write(STDOUT_FILENO, "server is busy\n", 15U);
					//		//	continue;
					//		//}
					//		throw sm::system_error("recv");
					//	}
					//}
				}
			}

	}; // class client

} // namespace sm

#endif // client_hpp
