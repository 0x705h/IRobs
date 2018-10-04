#include <stdio.h>
#include <sys/time.h>

int do_math(int a, int b);
int main(void) {

	struct timeval t;

	// setting volatile integer avoiding compiler
	// to optimize out this variable
	volatile int res = 0;

	gettimeofday(&t, NULL);
	printf("I will call a function...\n");
	if( res == 0 ) {
		res = do_math(256, t.tv_usec);
	}
	printf("Result :%d\n", res);

	return 0; 
}

int assign_constant_values() {
	int AAAA = 0x41414141;
	int BBBB = 0x42424242;

	return AAAA + BBBB;
}

int do_math(int a, int b) {

	if(a+b == 0) {
		return 1;
	}
	return a*b/a+b;
}

