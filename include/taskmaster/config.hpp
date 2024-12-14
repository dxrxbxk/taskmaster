#ifndef cconfig_hpp
#define cconfig_hpp

#include <string>


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	// -- forward declarations ------------------------------------------------

	/* program manager */
	class program_manager;


	// -- C O N F I G ---------------------------------------------------------

	class config final {

		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::config;


			// -- private members ---------------------------------------------

			/* path */
			std::string _path;


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

	}; // class config

} // namespace sm

#endif // config_hpp
