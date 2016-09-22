#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int sched_getcpu(void);

int main (int argc, char *argv[]) {
	int set_num_threads = 8;
	int cpu_alloc[set_num_threads];
	int migration_counter=0;
	omp_set_num_threads(set_num_threads);
	cpu_alloc[omp_get_thread_num()]= sched_getcpu();
	#pragma omp parallel
	{	
		cpu_alloc[omp_get_thread_num()]= sched_getcpu();
		int i;
		#pragma omp for
		for(i=0;i<48;i++){
//			printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			printf("i=%d ,thread id=%d , cpu = %d\n",i,omp_get_thread_num(),sched_getcpu());
			if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
                	{
                        	printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
                       		cpu_alloc[omp_get_thread_num()]= sched_getcpu();
                        	migration_counter++;
                	}
		}
	}
	printf("Total number of migration is %d \n", migration_counter);
	return 0;
}
