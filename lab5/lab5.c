
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

int main(int argc, char **argv)
{
	pid_t child_pid;
	int nbytes;
	int status;
	char readbuffer[80];
	int i=0;
	int fd[2][2], pid;
	if (pipe(fd[0]) == -1)
		error("sortedlist: error creating pipe");
	if ((pid = fork()) < 0)
		error("sortedlist: error while forking");
	if (pid > 0) {
		close(1);
		dup(fd[0][1]);
		close(fd[0][0]);
		close(fd[0][1]);
		nbytes = read(fd[0][0], readbuffer, sizeof(readbuffer));
		printf("%s",readbuffer);
		int wait_pid=waitpid(pid, &status, 0);
		printf("PID %d exited with status %d\n", wait_pid, status);
	}
	else if (pid == 0) {
		close(0);
		dup(fd[0][0]);
		close(fd[0][0]);
		close(fd[0][1]);
		char temp[100];
        strcpy(temp,"./");
        strcat(temp,argv[1]);
        char *cmd[] = {temp,argv[argc-1], NULL};
		printf("%d",execvp(cmd[0],cmd));
		printf("error\n");
		exit(-1);
	}

	return 0;
}
