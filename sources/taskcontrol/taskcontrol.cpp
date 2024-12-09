#include "taskcontrol/client.hpp"
#include "common/diagnostics/exception.hpp"
#include <iostream>


auto main(int ac, char** av) -> int {

	try {

		sm::client client{::in_addr{INADDR_ANY},
						  ::in_port_t{4242}};
		client.run();
	}

	catch (const sm::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
