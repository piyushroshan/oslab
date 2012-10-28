# include<stdio.h>
# include<pthread.h>
# include<unistd.h>
# include<semaphore.h>
sem_t chopstick[100];
int n;
void *thread_func(void	*arg)
{
	int no;
	no=(int)arg;
	int i,iteration=5;
	for(i=0;i<iteration;++i)
	{
		sem_wait(&chopstick[no]);
		sem_wait(&chopstick[(no+1)%n]);
		printf("Philosopher %d –> Begin eating\n",no);
		sleep(1);
		printf("Philosopher %d –> Finish eating\n",no);
		sem_post(&chopstick[no]);
		sem_post(&chopstick[(no+1)%n]);
	}
	pthread_exit(NULL);
}

int main()
{
	int i,res;
	pthread_t a_thread[100];
	printf("Enter the number of philosopher :\n");
	scanf("%d",&n);
	for(i=0;i<n;++i)
	{
		res=sem_init(&chopstick[i],0,0);
		if(res==-1)
		{
			printf("semaphore initialization failed\n");
		}
	}
	for(i=0;i<n;++i)
	{
		res=pthread_create(&a_thread[i],NULL,thread_func,(void*)i);
		if(res!=0)
		{
			printf("thread creation failed\n");
		}
		sem_post(&chopstick[i]);
	}
	for(i=0;i<n;++i)
	{
		res=pthread_join(a_thread[i],NULL);
		if(res!=0)
		{
			printf("thread join failed\n");
		}
	}
	printf("Program terminates successfully\n");
return 0;
}
