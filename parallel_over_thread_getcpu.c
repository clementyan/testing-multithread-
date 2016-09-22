#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int sched_getcpu(void);

int main (int argc, char *argv[]) {

	omp_set_num_threads(8);//my nb maximum of thread 4;
	int i=0;
	for(;i<4;i++){
		#pragma omp parallel
		{
			printf("thread id=%d , cpu = %d\n",omp_get_thread_num(),sched_getcpu());
		}
		printf("========================\n");
	}
	return 0;

}
