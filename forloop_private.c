#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

int sched_getcpu(void);

int main (int argc, char *argv[]) {

	omp_set_num_threads(4);
	int i=1,n=100000;
	float a[100000], b[100000], sum;
 	for (i=0; i < n; i++) {
                a[i] = i*1.0;
                b[n-1-i] = i*1.0;
        }
        sum = 0.0;
	#pragma omp parallel reduction(+:sum) private(i) shared(a,b)//private(sum)//default(none) shared(sum)//default(shared) //private(i)
	{
	#pragma omp for
	for(i=1;i<200;i++)
	{
		printf("sum=%f, a[%d]=%f, b[%d]=%f\n",sum,i,a[i],i,b[i]);
		sum = sum + (a[i] * b[i]);
		//sum+=i;
		
		printf("sum=%f, i=%d, thread id=%d, cpu = %d\n",sum,i,omp_get_thread_num(),sched_getcpu());
	}
	}
	printf("sum=%f\n",sum);
	return 0;
}
