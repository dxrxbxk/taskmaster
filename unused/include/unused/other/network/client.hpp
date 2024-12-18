#ifndef client_hpp
#define client_hpp

#include "events/monitor.hpp"
#include "network/socket.hpp"
#include "reader.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C L I E N T ---------------------------------------------------------

	class client final : public sm::listener {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::client;


			// -- private members ---------------------------------------------

			/* socket */
			sm::socket _socket;

			/* buffer */
			std::string _buffer;


			// -- private methods ---------------------------------------------

			/* disconnect */
			auto _disconnect(sm::taskmaster&) noexcept -> void;

			/* to hex */
			auto _to_hex(const char* data, const sm::usize& size) -> std::string {

				// hex string
				std::string hex;

				// loop over data
				for (sm::usize i = 0U; i < size; ++i) {

					// hex
					char buff[4U];
					snprintf(buff, 4U, "%02x ", data[i]);

					// append
					hex.append(buff, 3U);
				}

				return hex;
			}


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client(void) noexcept = default;

			/* socket constructor */
			client(sm::socket&&) noexcept;

			/* deleted copy constructor */
			client(const self&) = delete;

			/* move constructor */
			client(self&&) noexcept = default;

			/* destructor */
			~client(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public overrides --------------------------------------------

			/* fd */
			auto fd(void) const noexcept -> int override;

			/* on event */
			auto on_event(const sm::event&, sm::taskmaster&) -> void override;

	}; // class client

} // namespace sm

#endif // client_hpp
