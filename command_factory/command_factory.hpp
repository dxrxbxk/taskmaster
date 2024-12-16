#ifndef COMMAND_FACTORY_HPP
# define COMMAND_FACTORY_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include "test.hpp"

class command;

class commandfactory {
	public:
		using command_ptr = std::unique_ptr<command>;
		using command_map = std::unordered_map<std::string, command_ptr>;


		static command_ptr create(const std::string& name) {
			auto& instance = shared();
			auto it = instance._commands.find(name);
			if (it == instance._commands.end()) {
				return nullptr;
			}
			return it->second->clone();
		}


	private:

		static commandfactory& shared() {
			static commandfactory instance;
			return instance;
		}

		commandfactory() {
			_commands["test"] = std::make_unique<test>();
		}

		~commandfactory() = default;

		commandfactory(const commandfactory&) = delete;
		commandfactory& operator=(const commandfactory&) = delete;
		commandfactory(commandfactory&&) = delete;
		commandfactory& operator=(commandfactory&&) = delete;

		command_map _commands;

};

#endif // COMMAND_FACTORY_HPP

