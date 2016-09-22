#define _GNU_SOURCE
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <sched.h>
#include <time.h>
#include <sys/syscall.h>

#define size 10

void MultiplyMatrix();
void printSchedulerName(int PID);

int MatrixA[size][size]={0};
int MatrixB[size][size]={0};
int MatrixC[size][size]={0};
 
pid_t getpid();


int main(int argc, char** argv)
{
	int set_num_threads = 8;	
	srand(time(NULL));
	int i,j,h, n=100000;
	long a[100000], b[100000],sum;
	time_t second, second2;
	
	cpu_set_t set;
	int ret,PID, cpu, prio;
	int cpu_alloc[set_num_threads];
	int migration_counter=0;
	
	CPU_ZERO(&set);//Clears set, so that it contains no CPUs.
	for(i=3; i >0; i--)
		CPU_SET(i, &set);//Add CPU cpu to set.
//ref:http://linux.die.net/man/3/cpu_zero
struct sched_param sp;
sp.sched_priority=sched_get_priority_max(SCHED_FIFO);
ret=sched_setscheduler(0,SCHED_FIFO,&sp);

#pragma omp parallel for num_threads(set_num_threads) private(PID,ret,set)
        for(i=0;i<omp_get_num_threads();i++)
        {
                PID=getpid()+omp_get_thread_num();
                CPU_ZERO(&set);
                CPU_SET(i/2,&set);
                ret=sched_setaffinity(PID,sizeof(cpu_set_t),&set);
        }
	
//	ret=sched_setaffinity(0, sizeof(cpu_set_t), &set);				
			
	omp_set_num_threads(set_num_threads);

	second2 = time(NULL);	

	/* Some initializations */

	for (i=0; i < n; i++) {
		a[i] = i*1.0;
		b[n-1-i] = i*1.0;
	}
	sum = 0;
	int chunk=10000;
	//#pragma omp parallel default(none) shared(a,b,n,chunk,migration_counter) private(i,cpu_alloc) reduction(+:sum)
	#pragma omp parallel private(i) reduction(+:sum)
	{ 
	cpu_alloc[omp_get_thread_num()]= sched_getcpu();
	#pragma omp for //shared(a,b,n,chunk) private(i) reduction(+:sum) schedule(static,chunk)
	for (i=0; i < n; i++)
	{
	//	#pragma omp atomic
		sum +=  (a[i] * b[i]);

		if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
		{
			printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
			cpu_alloc[omp_get_thread_num()]= sched_getcpu();
			migration_counter++;
		}

	}
	}
	printf("\n Sum = %ld\n",sum);

	second = time(NULL);	
	printf("\n total seconds : %ld\n",second-second2);
	

	for(j=0;j<size;j++)
	{
		for(i=0;i<size;i++)
		{
			MatrixA[i][j] = i*j;
			MatrixB[i][j] = i+j;
			
			if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
			{
				printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
				cpu_alloc[omp_get_thread_num()]= sched_getcpu();
				migration_counter++;	
			}
		}
	}

	printf("******************************************************\n");
	printf("Matrix A:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) 
	    printf("%d \t", MatrixA[i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");
	printf("Matrix B:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) 
	    printf("%d \t", MatrixB[i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");

	omp_set_num_threads(set_num_threads);
	second2 = time(NULL);

	for (i=0; i < 3000; i++)
	{
		for (j=0; j<3000; j++) 
		{
	//		MultiplyMatrix();
	int ii=0, jj=0, k=0;
#pragma omp parallel default(shared) private(ii,jj,k)
{
	cpu_alloc[omp_get_thread_num()]= sched_getcpu();
	#pragma omp for schedule(static)
	for(ii=0;ii<size;ii++)
	{
		for(jj=0;jj<size;jj++)
		{
			MatrixC[ii][jj]=0;
			for(k=0;k<size;k++)
			{
				MatrixC[ii][jj] = MatrixC[ii][jj]+MatrixB[ii][jj]* MatrixA[k][jj];
				//printf("%d  %d   %d ", MatrixC[ii][jj],MatrixB[ii][jj],MatrixA[k][jj]);
			}
		}
	}

			if(sched_getcpu()!=cpu_alloc[omp_get_thread_num()])
			{
	//			printf("The thread %d is moved from CPU%d to CPU%d \n", omp_get_thread_num(), cpu_alloc[omp_get_thread_num()], sched_getcpu());
				cpu_alloc[omp_get_thread_num()]= sched_getcpu();
				migration_counter++;				
			}
}
		}

	}

	
	/*** Print results ***/

	printf("******************************************************\n");
	printf("Result Matrix:\n");
	for (i=0; i<size; i++)
	{
	  for (j=0; j<size; j++) 
	    printf("%d \t", MatrixC[i][j]);
	  printf("\n"); 
	 }
	printf("******************************************************\n");
	printf ("Done.\n");

	printf("Total number of migration is %d \n", migration_counter);

	second = time(NULL);	
	printf("\n total seconds : %ld\n",second-second2);

}

void MultiplyMatrix()
{
	int i=0, j=0, k=0;
//#pragma omp parallel for default(shared) private(i,j,k) reduction(+:MatrixC)
	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			MatrixC[i][j]=0;
			for(k=0;k<size;k++)
			{
				MatrixC[i][j] = MatrixC[i][j]+MatrixB[i][j]* MatrixA[k][j];
				//printf("%d  %d   %d ", MatrixC[i][j],MatrixB[i][j],MatrixA[k][j]);
			}
		}
	}	
}

void printSchedulerName(int PID){	
	int policy = sched_getscheduler(PID);	
	switch(policy){		
	  case SCHED_OTHER:
		printf("Thread: %d, Policy: Normal\n", PID);
		break;
	  case SCHED_RR:
		printf("Thread: %d, Policy: Round-Robin\n", PID);
		break;
	  case SCHED_FIFO:
		printf("Thread: %d, Policy: FIFO\n", PID);
		break;	
	  case -1:
		perror("sched_getscheduler");
		break;
	  default:
		printf("Thread: %d, Policy: Unknown\n", PID);
		break;	
	}
}
