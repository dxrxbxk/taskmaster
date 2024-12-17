#ifndef command_factory_hpp
#define command_factory_hpp

#include <string>
#include <unordered_map>
#include <memory>

#include "common/resources/unique_ptr.hpp"

#include "taskmaster/builtins/help.hpp"
#include "taskmaster/builtins/start.hpp"
#include "taskmaster/builtins/stop.hpp"
#include "taskmaster/builtins/list.hpp"


// -- S M  N A M E S P A C E --------------------------------------------------

namespace sm {


	class command_factory final {


		private:

			// -- private types -----------------------------------------------

			/* self type */
			using self = sm::command_factory;

			using functor = auto (*)(std::vector<std::string>&&) -> sm::unique_ptr<sm::command>;


			using command_ptr = sm::unique_ptr<command>;
			using command_map = std::unordered_map<std::string, functor>;


			// -- private members ---------------------------------------------

			command_map _commands;


			// -- private lifecycle -------------------------------------------

			/* default constructor */
			command_factory(void)
			: _commands{} {

				_commands["stop" ] = _make_unique<sm::cmd::stop>;
				_commands["list" ] = _make_unique<sm::cmd::list>;
				_commands["start"] = _make_unique<sm::cmd::start>;
				_commands["help" ] = _make_unique<sm::cmd::help>;
			}

			/* deleted copy constructor */
			command_factory(const self&) = delete;

			/* deleted move constructor */
			command_factory(self&&) = delete;

			/* destructor */
			~command_factory(void) noexcept = default;


			// -- private assignment operators --------------------------------

			/* deleted copy assignment operator */
			auto operator=(const self&) -> self& = delete;

			/* deleted move assignment operator */
			auto operator=(self&&) -> self& = delete;


		public:

			// -- public static methods ---------------------------------------

			static auto create(std::vector<std::string>&& argv) -> command_ptr {

				auto& instance = self::_shared();

				auto it = instance._commands.find(argv[0U]);

				if (it != instance._commands.end()) {
					return it->second(std::move(argv));
				}

				// not found
				throw sm::system_error{"invalid command"};
			}



			template <typename T>
			static auto _make_unique(std::vector<std::string>&& argv) -> sm::unique_ptr<command> {
				return sm::make_unique<T>(std::move(argv));
			}


		private:

			/* shared instance */
			static auto _shared(void) -> self& {
				static self instance;
				return instance;
			}



	}; // class command_factory

} // namespace sm

#endif // command_factory_hpp
