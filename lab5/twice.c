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
int main(int argc,char** argv){
  int num=atoi(argv[argc-1]);
if(argc==2)
{
  
  printf("%d %d",num*2,getpid());
  return 0;
}
else
{
  printf("%d",getpid());
  char temp[80];
  strcpy(temp,"./");
  num=2*num;
  char buffer[80];
  char **rest;
  sprintf(buffer,"%d",num);
  printf("BUFFER %s",buffer);
  rest=&argv[1];

  printf(" RESTI %s",rest[2]);
//   rest[argc-3]=buffer;
//   printf("RESTF %s",*rest);
//   strcat(temp,argv[1]);
//   printf("%d",execvp(temp,rest));
  printf("error\n");
  exit(-1);
return 0;
}
}
