#include <unistd.h>
#include <cstdlib>
#include <ctime>


int	main(void)
{
	sleep(3);

	static bool flag = false;
	if (!flag) {
		::srand(static_cast<unsigned int>(::time(0)));
		flag = true;
	}

	return ::rand() % 10;
}
