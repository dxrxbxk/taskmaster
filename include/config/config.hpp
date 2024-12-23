#ifndef cconfig_hpp
#define cconfig_hpp

#include "inotify.hpp"
#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* program manager */
	class program_manager;


	// -- C O N F I G ---------------------------------------------------------

	class config final : public sm::notifiable {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::config;


			// -- private members ---------------------------------------------

			/* path */
			std::string _path;


			// -- private static methods --------------------------------------

			/* parse */
			static auto _parse(const sm::unique_fd&, sm::program_manager&) -> void;


		public:

			// -- public lifecycle --------------------------------------------

			/* default constructor */
			config(void);

			/* path constructor */
			config(const std::string&);

			/* move path constructor */
			config(std::string&&) noexcept;

			/* copy constructor */
			config(const self&) = default;

			/* move constructor */
			config(self&&) noexcept = default;

			/* destructor */
			~config(void) noexcept = default;


			// -- public assignment operators ---------------------------------

			/* copy assignment operator */
			auto operator=(const self&) -> self& = default;

			/* move assignment operator */
			auto operator=(self&&) noexcept -> self& = default;


			// -- public methods ----------------------------------------------

			/* parse */
			auto parse(sm::program_manager&) -> void;


			// -- public modifiers --------------------------------------------

			/* path */
			auto path(const std::string&) -> void;

			/* move path */
			auto path(std::string&&) noexcept -> void;


			// -- public overrides --------------------------------------------

			/* path */
			auto path(void) const noexcept -> const char* override;

			/* on event */
			auto on_event(const ::uint32_t&, sm::taskmaster&) -> void override;

	}; // class config

} // namespace sm

#endif // config_hpp
