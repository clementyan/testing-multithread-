#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]) {
	int i,a[10],b[10],c[10]={0};
	for(i=0;i<10;i++)
	{
		a[i]=i;
		b[i]=2*i;	
	}
	omp_set_num_threads(2);
	#pragma omp parallel
	{
		#pragma omp sections
		{
		#pragma omp section
		{
			int i;
			for(i=0;i<5;i++)
			{
				c[i]=a[i]+b[i];
				printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		#pragma omp section
		{
			int i;
			for(i=5;i<10;i++)
			{
				c[i]=a[i]+b[i];
				printf("i=%d thread id=%d\n",i,omp_get_thread_num());
			}
		}
		}
	}
	for(i=0;i<10;i++)
	{
		printf("c[%d]= %d\n",i,c[i]);
	}
	return 0;
}
