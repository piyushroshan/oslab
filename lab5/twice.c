#include<stdio.h>
int main(int argn,char** argv){
int num=atoi(argv[1]);
printf("%d %d",num*2,getpid());
return 0;
}
