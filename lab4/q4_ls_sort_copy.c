
#include <stdlib.h>	
#include <unistd.h>   // for execlp 
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdarg.h>

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
	char *dir;
	if (argc == 1)
		dir = NULL;
	else
		dir = argv[1];

	int fd[2], pid;
	if (pipe(fd) == -1)
		error("sortedlist: error creating pipe");
	if ((pid = fork()) < 0)
		error("sortedlist: error while forking");

	if (pid > 0) {
		close(1);
		dup(fd[1]);
		close(fd[0]);
		close(fd[1]);
		execlp("ls", "ls", dir, NULL);
	}
	else if (pid == 0) {
		close(0);
		dup(fd[0]);
		close(fd[0]);
		close(fd[1]);
		execlp("sort", "sort", NULL);
	}

	return 0;
}
