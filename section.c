#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int sched_getcpu(void);

int main (int argc, char *argv[]) {

	omp_set_num_threads(8);
	#pragma omp parallel
	{
		#pragma omp sections
		{
		#pragma omp section
		{
			int i;
			for(i=0;i<5;i++)
			{
				printf("thread_section1 id=%d , cpu = %d\n",omp_get_thread_num(),sched_getcpu());
//				printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		#pragma omp section
		{
			int i;
			for(i=5;i<11;i++)
			{
				printf("thread_section2 id=%d , cpu = %d\n",omp_get_thread_num(),sched_getcpu());
				//printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		}
	}
	return 0;
}
