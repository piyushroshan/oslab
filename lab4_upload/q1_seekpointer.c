#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h> 
#include <stdlib.h>
int main(int argc,const char **argv)
{
    int file=0;
    if((file=open(argv[1],O_RDONLY)) < -1)
               { printf("error opening"); return 1;}
 
     char buffer[100];
     if(read(file,buffer,100)<=-1)
        { 
            printf("error reading\n");  return 1;
        }
        printf("%s\n",buffer);
	    printf("\n------------------------------\n");
	    char *buffer2; 
     if(lseek(file,atoi(argv[2]),SEEK_SET) < 0){printf("seek error\n"); return 1;
}
        if(read(file,buffer2, atoi(argv[3]))<=-1 ) {printf("read error2\n"); return 1;}
buffer2[atoi(argv[3])]='\0';    
 printf("%s\n",buffer2); 
        return 0;
}
