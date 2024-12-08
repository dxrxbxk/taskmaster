#ifndef os_hpp
#define os_hpp


#if defined(_WIN32) || defined(_WIN64)
	#define os_windows

#elif defined(__linux__)
	#define os_linux

#elif defined(__APPLE__) && defined(__MACH__)
	#define os_macos
#endif


#endif // os_hpp
