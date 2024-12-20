#include <stdlib.h>

int __exit(void) {
	__asm__ __volatile__ (
		"movq $60, %%rax\n"
		"xorq %%rdi, %%rdi\n"
		"syscall"
		: : : "rax", "rdi");
}

long	sys_write(int fd, const void *buf, unsigned long count)
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

int main(void) {



	char *str = malloc(100);

	__builtin_strcpy(str, "Hello, World!\n");
	sys_write(1, str, __builtin_strlen(str));
	__exit();

}
