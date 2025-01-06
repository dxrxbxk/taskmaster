#ifndef client_manager_hpp
#define client_manager_hpp

#include "network/client.hpp"
#include <unordered_map>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- C L I E N T  M A N A G E R ------------------------------------------

	class client_manager final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::client_manager;


			// -- private members ---------------------------------------------

			/* clients */
			std::unordered_map<int, sm::client> _clients;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			client_manager(void) noexcept(
					noexcept(std::unordered_map<int, sm::client>{})) = default;

			/* deleted copy constructor */
			client_manager(const self&) = delete;

			/* move constructor */
			client_manager(self&&) noexcept = default;

			/* destructor */
			~client_manager(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public modifiers --------------------------------------------

			/* add */
			auto add(sm::client&& client) -> sm::client& {

				const int key = client.fd();

				auto pair = _clients.emplace(key, static_cast<sm::client&&>(client));

				return pair.first->second;
			}

			/* add */
			auto add(sm::socket&& socket) -> sm::client& {

				const int key = socket;

				auto pair = _clients.emplace(key, static_cast<sm::socket&&>(socket));

				return pair.first->second;
			}

			/* remove */
			auto remove(const int& fd) -> void {
				_clients.erase(fd);
			}

			/* remove */
			auto remove(const sm::client& client) noexcept -> void {
				_clients.erase(client.fd());
			}

	}; // class client_manager

} // namespace sm

#endif // client_manager_hpp
