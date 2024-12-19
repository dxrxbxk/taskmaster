#include <stdio.h>
#include <limits.h>

int main() {

	int a = INT_MAX;

	__auto_type b = a * 10;

	if (__builtin_add_overflow(a, 0, &a) != 0) {
		printf("Overflow\n");
	} else {
		printf("result: %d\n", a);
	}
}
