#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {

	omp_set_num_threads(8);//my nb maximum of thread 4; 
	#pragma omp parallel
	{
		printf("thread id=%d\n",omp_get_thread_num());
	}
	return 0;
}
