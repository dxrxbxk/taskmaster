#ifdef LIBC
#include <stdlib.h>
#endif

int ___exit(void) {
	__asm__ __volatile__ (
		"movq $60, %%rax\n"
		"xorq %%rdi, %%rdi\n"
		"syscall"
		: : : "rax", "rdi");

}

long	___write(int fd, const void *buf, unsigned long count)
{
	long ret;
	__asm__ __volatile__ (
		"movq %0, %%rdi\n"
		"movq %1, %%rsi\n"
		"movq %2, %%rdx\n"
		"movq $1, %%rax\n"
		"syscall"
		: : "g" (fd), "g" (buf), "g" (count));
	__asm__ ("movq %%rax, %0" : "=r" (ret));

	return ret;
}

#ifdef LIBC
int main(void) {

	char *str = malloc(100);
	__builtin_strcpy(str, "Hello from LibC!\n");
	___write(1, str, __builtin_strlen(str));
	___exit();
	return 0;
}
#else
void _start(void) {
	char *str = "Hello from _start!\n";
	___write(1, str, __builtin_strlen(str));
	___exit();
}
#endif
