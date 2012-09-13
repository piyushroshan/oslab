 #include<stdio.h>

int main(int argn,char** argv){

int num=atoi(argv[1]);

printf("%lf %d",num*0.5,getpid());

return 0;

}
