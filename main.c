#include <stdio.h>
#include <sys/time.h>

int do_math(int a, int b);
int main(void) {

	struct timeval t;

	int res = 0;

	gettimeofday(&t, NULL);
	printf("I will call a function...\n");
	res = do_math(256, t.tv_usec);

	printf("Result :%d\n", res);

	return 0; 
}

int do_math(int a, int b) {

	if(a+b == 0) {
		return 1;
	}
	return a*b/a+b;
}

