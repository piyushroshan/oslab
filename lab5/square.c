
#include <stdlib.h>	
#include <unistd.h>   // for execlp 
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

void error(char *message, ...)
{
	va_list args;

	va_start(args, message);
	fprintf(stderr, "error: ");
	vfprintf(stderr, message, args);
	fprintf(stderr, "\n");
	va_end(args);
	exit(1);
}

int main(int argc,char** argv)
{
	float num=atof(argv[argc-1]);
	num=num*num;
	if(argc>2)
	{
		printf("Square %d\t",getpid());
		char temp[100];
		strcpy(temp,"./");
		strcat(temp,argv[1]);   

		char buffer[5];
		printf("\n");
		sprintf(buffer, "%lf", num);
		strcpy(argv[argc-1],buffer);
		argv[1]=temp;
		//sleep(1);
		printf("%d",execvp(argv[1],&argv[1]));
	}
	else if (argc==2)
	{
		printf("Result: %lf\tProcessid: %d\n",num,getpid());
	}
	else
	{
		printf("Invalid argument\n");
	}

	return 0;
}