#ifndef COMMAND_HPP
# define COMMAND_HPP

#include <memory>

class command {
	public:
		command() = default;
		using ret_type = void;
		virtual ~command() = default;
		virtual void execute() const = 0;
		virtual std::unique_ptr<command> clone() const = 0;
};

#endif // COMMAND_HPP
