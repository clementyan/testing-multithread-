#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {

	omp_set_num_threads(4);
	#pragma omp parallel
	{	int i;
		#pragma omp for
		for(i=0;i<100;i++){
			printf("i=%d thread id=%d\n",i,omp_get_thread_num());
		}
		/*
		printf("===========================\n");
		#pragma omp sections
		{
		#pragma omp section
		{
			int i;
			for(i=0;i<50;i++)
			{
				printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		#pragma omp section
		{
			int i;
			for(i=50;i<101;i++)
			{
				printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		}
	*/
	}
	return 0;
}
