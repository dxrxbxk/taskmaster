#include "taskcontrol/client.hpp"
#include <iostream>


auto main(int ac, char** av) -> int {

	try {

		sm::client client{::in_addr{INADDR_ANY},
						  ::in_port_t{4242}};
		client.run();
	}

	catch (const std::exception& e) {
		std::cerr << "error: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
