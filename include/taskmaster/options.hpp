#ifndef options_hpp
#define options_hpp

#include "common/types.hpp"
#include <string>
#include <netinet/in.h>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- O P T I O N S -------------------------------------------------------

	class options final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::options;


			// -- private data ------------------------------------------------

			/* program */
			const std::string _program;

			/* config */
			std::string _config;

			/* port */
			::in_port_t _port;

			/* daemon */
			bool _daemon;


			struct key_function final {

				const char* key;
				auto (self::*func)(const char*) -> void;
				bool has_argument;
			};


			auto _handle_port(const char*) -> void;

			auto _handle_daemon(const char*) -> void;

			auto _handle_config(const char*) -> void;


			// -- private static members --------------------------------------

			/* keys */
			static constexpr key_function _table[] {

				// port
				{"-p",       &self::_handle_port, true},
				{"--port",   &self::_handle_port, true},

				// daemon
				{"-d",       &self::_handle_daemon, false},
				{"--daemon", &self::_handle_daemon, false},

				// config
				{"-c",       &self::_handle_config, true},
				{"--config", &self::_handle_config, true}
			};



			// -- private methods ---------------------------------------------

			/* retrieve handler */
			auto _retrieve_handler(const char* const) -> const key_function&;





		public:

			// -- public lifecycle --------------------------------------------

			/* deleted default constructor */
			options(void) = delete;

			/* argc / argv constructor */
			options(const int&, char**);

			/* deleted copy constructor */
			options(const self&) = delete;

			/* deleted move constructor */
			options(self&&) = delete;

			/* destructor */
			~options(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


			// -- public accessors --------------------------------------------

			/* port */
			auto port(void) const noexcept -> ::in_port_t;

			/* daemon */
			auto daemon(void) const noexcept -> bool;

			/* config */
			auto config(void) const noexcept -> const std::string&;

			/* config */
			auto config(void) noexcept -> std::string&&;

			/* has config */
			auto has_config(void) const noexcept -> bool;

	}; // class options

} // namespace sm

#endif // options_hpp
