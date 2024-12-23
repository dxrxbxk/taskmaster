#include <iostream>
#include <signal.h>


template <int signal>
auto _ignore(void) -> void {
	if (::signal(signal, SIG_IGN) == SIG_ERR)
		throw std::runtime_error("Failed to ignore signal");
}

template <int... signals>
auto ignore_signal(void) -> void {
	(_ignore<signals>(), ...);
}

int main(int, char** av) {

	ignore_signal<SIGINT, SIGTERM, SIGQUIT, SIGTSTP>();

	while (true) {
		std::cout << av[0] << " is running\r\n";
		sleep(1);
	}

	return 0;
}
 
