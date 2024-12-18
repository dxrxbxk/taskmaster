#ifndef addr_hpp
#define addr_hpp

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- A D D R -------------------------------------------------------------

	class addr {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::addr;


			// -- private members ---------------------------------------------

			/* storage */
			::sockaddr_storage _storage;

			/* size */
			::socklen_t _size;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			addr(void) noexcept;

			/* ipv4 constructor */
			addr(const ::in_addr&, const ::in_port_t&) noexcept;

			/* ipv6 constructor */
			addr(const ::in6_addr&, const ::in_port_t&) noexcept;

			/* local constructor */
			addr(const char*) noexcept;

			/* copy constructor */
			addr(const self&) noexcept = default;

			/* move constructor */
			addr(self&&) noexcept = default;

			/* destructor */
			~addr(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) noexcept -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public accessors --------------------------------------------

			/* size */
			auto size(void) noexcept -> ::socklen_t&;

			/* const size */
			auto size(void) const noexcept -> const ::socklen_t&;

			/* as ipv4 */
			auto as_ipv4(void) noexcept -> ::sockaddr_in&;

			/* const as ipv4 */
			auto as_ipv4(void) const noexcept -> const ::sockaddr_in&;

			/* as ipv6 */
			auto as_ipv6(void) noexcept -> ::sockaddr_in6&;

			/* const as ipv6 */
			auto as_ipv6(void) const noexcept -> const ::sockaddr_in6&;

			/* as local */
			auto as_local(void) noexcept -> ::sockaddr_un&;

			/* const as local */
			auto as_local(void) const noexcept -> const ::sockaddr_un&;

			/* as sockaddr */
			auto as_sockaddr(void) noexcept -> ::sockaddr&;

			/* const as sockaddr */
			auto as_sockaddr(void) const noexcept -> const ::sockaddr&;

	}; // class addr

} // namespace sm

#endif // addr_hpp
