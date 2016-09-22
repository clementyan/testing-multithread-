#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int sched_getcpu(void);

int main (int argc, char *argv[]) {

	omp_set_num_threads(4);
	#pragma omp parallel
	{	int i;
		#pragma omp for
		for(i=0;i<50;i++){
//			printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			printf("i=%d ,thread id=%d , cpu = %d\n",i,omp_get_thread_num(),sched_getcpu());
		}
	}
	return 0;
}
