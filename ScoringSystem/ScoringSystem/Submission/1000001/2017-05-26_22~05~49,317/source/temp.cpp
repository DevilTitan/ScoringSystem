#include "hello.h"
#include <stdio.h>
#include <conio.h>

int foo();

int main()
{
	printf("hello world");
 	FILE *fp;
    	fp = fopen("output.txt", "a");
	      fprintf(fp, "%dsdkjfldfj\n", N);
	fclose(fp);
	foo();
	return 0;
}
