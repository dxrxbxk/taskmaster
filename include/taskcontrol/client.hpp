#ifndef client_hpp
#define client_hpp

#include "common/diagnostics/exception.hpp"
#include "common/network/socket.hpp"
#include "common/network/addr.hpp"
#include "taskcontrol/readline.hpp"


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
				_socket.timeout(2'000, SO_RCVTIMEO);
			}


			// -- public methods ----------------------------------------------

			/* run */
			auto run(void) -> void {

				sm::readline2 rl;


				// read from stdin
				while (true) {

					rl.read();

					if (rl.line() == "quit"
					 || rl.line() == "exit")
						break;

					if (::send(_socket, rl.line().data(), rl.line().size(), 0) == -1)
						throw sm::system_error("send");

					continue; // CCONTINUUUUUUUUUUUUUUU

					char buffer[1024U];

					unsigned attempts = 0U;

					for (unsigned i = 0U; i < 3U; ++i) {

						const auto bytes = ::recv(_socket, buffer, sizeof(buffer), 0);

						if (bytes == -1) {
							if (errno == EAGAIN || errno == EWOULDBLOCK) {
								::write(STDOUT_FILENO, "server is busy\n", 15U);
								continue;
							}
							throw sm::system_error("recv");
						}
					}
				}
			}

	}; // class client

} // namespace sm

#endif // client_hpp
