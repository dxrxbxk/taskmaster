#ifndef config_hpp
#define config_hpp


// has built-in
#if defined(__has_builtin)
#	define sm_has_builtin(b) __has_builtin(b)
#else
#	define sm_has_builtin(b) 0
#endif

#endif // config_hpp
