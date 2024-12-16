#include "command_factory.hpp"


int main(int argc, char** argv) {

	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " <command>" << std::endl;
		return 1;
	}

	auto cmd = commandfactory::create(argv[1]);

	if (cmd) {
		cmd->execute();
	} else {
		std::cout << "command not found" << std::endl;
	}
	return 0;
}
