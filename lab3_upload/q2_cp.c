/*
 * 1. Write a C program to implement 'cp' command in linux.
 * (Source and destination files should be taken from command line)
 */

#include <unistd.h>	//read, write
#include <fcntl.h>	//open, creat
#include <sys/syscall.h>	//error
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define BUFSIZE 1024


int main(int argc, char *argv[])
{
  struct stat stat_buf;
	if (argc != 3)
		error("Usage: cp source destination");

	char buf[BUFSIZE];
	int source, dest, count;

	if ((source = open(argv[1], O_RDONLY)) == -1)
		error("cp: failed to open %s", argv[1]);
	 fstat (source, &stat_buf);
	if ((dest = creat(argv[2], stat_buf.st_mode)) == -1)
		error("cp: failed to open %s for writing", argv[2]);

	while ((count = read(source, buf, BUFSIZE)) > 0)
		if ((write(dest, buf, count)) != count)
			error("cp: write error on file %s", argv[2]);

	return 0;
}
