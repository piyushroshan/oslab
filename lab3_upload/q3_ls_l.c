#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char **argv ) {
struct stat fileStat;

if(stat(argv[1],&fileStat) < 0)
{ printf("stat failed");
  return 1;
}
    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf(" %ld",fileStat.st_nlink);
    printf(" %s",getpwuid(fileStat.st_uid)->pw_name  );
    printf(" %s",getgrgid(fileStat.st_gid)->gr_name  );
    printf(" %ld",fileStat.st_size);
    char buffer[100];
    strftime(buffer,100,'%b %d %H:%M:%S', localtime(&fileStat.st_ctime)));
    printf("%s", buffer);
    printf(" %s",ctime(&fileStat.st_ctime));
	printf(" %s\n", argv[1]);
return 0;
}




