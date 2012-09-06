#include <fcntl.h>					//required for file operations like open(), stat() etc.
#include <dirent.h>					//for traversing directories. contains defn of struct dirent and fns opendir, readdir etc.
#include <stdio.h>		
#include <stdlib.h>				
#include <sys/stat.h>					//contains macro S_ISDIR	
#include <string.h>					//contains string operations strcpy and strcat
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>



void prnsize(off_t);					//fn to print the size passed as argument
off_t dir_du(char *, char *);				//function to traverse a directory and return its size takes argument filename and path


int main(int argc, char* argv[])				
{
	struct stat file;						//struct to hold file info
	off_t size;							//datatype for handling sizes
	char path[100];						
	
	if(argc==1)
	{
		size=dir_du(".", ".");
		return 0;
	}
	
	if(argc!=2)
	{
		printf("Format : %s <filename>\n", argv[0]);
		exit(1);
	}
	
	if(stat(argv[1], &file)<0)						//stat fn returns -1 on failure.
	{
		fprintf(stderr, "Error opening file %s \n", argv[1]);
		exit(1);
	}
	
	if(!S_ISDIR(file.st_mode))						//if argument is not directory, print size of the file only.
	{
		printf("\t%s\n", argv[1]);
	}
	
	else 									//else traverse the directory and prints its size.
		size=dir_du(argv[1], argv[1]);
		
	return 0;
}


void prnsize(off_t size)
{
	printf("%ld", (long)size);						//typecast off_t to long
	return;
}


off_t dir_du(char * dir_name, char *path)					//takes current directory name and its path as arguments
{
	off_t dir_size=0;							//variable holding current directory's size
	DIR *dir;
	struct dirent *dir_entry;						//variable to store the details of each directory entry as it is read.
	struct stat fileStat;							//variable to store file details(contains off_t value) of each directory entry.
		
	dir=opendir(dir_name);							//open dir_name directory and store stream info in variable dir.
	int fd = open(dir_name, O_RDONLY);
	if(fstat (fd, &fileStat)== -1 )					//get file info of given directory entry, fn returns -1 on failure.
		{
			fprintf(stderr, "Error getting info for file %s\n", dir_entry->d_name);
			exit(1);
		}
		
	
	close(fd);	
		
	if(chdir(dir_name)==-1)							//change working directory of program to dir_name.
	{
		fprintf(stderr, "Error switching to directory %s\n", dir_name);
		exit(1);
	}
		
	while((dir_entry=readdir(dir))!=NULL)					//read directory entries from dir one by one
	{	
		fd=open(dir_entry->d_name, O_RDONLY);
		
		if(fstat(fd, &fileStat)==-1)			//get file info of given directory entry, fn returns -1 on failure.
		{
			fprintf(stderr, "Error getting info for file %s\n", dir_entry->d_name);
			exit(1);
		}
		
		if(!S_ISDIR(fileStat.st_mode))	//if current entry is not a directory in itself, simply add its size
		{			
			char temp[1000];
			strcpy(temp, path);	
		printf("%s\n", strcat(strcat(temp, "/"), dir_entry->d_name));
	}
		else if((strcmp(dir_entry->d_name, "..")==0)||(strcmp(dir_entry->d_name, ".")==0))		//skip entry if it is . or ..
			continue;
		else
		{
			char temp[1000];
			strcpy(temp, path);			
			dir_du(dir_entry->d_name, strcat(strcat(temp, "/"), dir_entry->d_name));		
		}
		close(fd);
			
	}

	if(chdir("..")==-1)
	{
		fprintf(stderr, "Error switching to parent directory %s\n", dir_name);
		exit(1);
	}
	closedir(dir);								//close the directory stream to clear memory.
	return dir_size;
}
			
		
		
	
	
