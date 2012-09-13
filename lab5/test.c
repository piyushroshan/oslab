#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>   // for execlp 
#include <sys/syscall.h>
#include <errno.h>
#include <sys/types.h>
#include <stdarg.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
int main(int argn,char** argv){
int num=atoi(argv[argn-1]);
num=num*2;
printf("%d",getpid());
char **rest;
char temp[100];
strcpy(temp,"./");
strcat(temp,argv[1]);   
if(argn>2){
strncpy(rest,argv+1,argn-2);
char buf[5];
//itoa(num, buf, 10);
sprintf(buf, "%d", num);
strcat(*rest,buf);
char *cmd[] = {temp,rest, NULL};
printf("%d",execvp(cmd[0],cmd));
}
else{
printf("%d %d",num,getpid());
}

return 0;
}