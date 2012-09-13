#include<stdio.h>
#include<iostream>
#include<string>
using namespace std;
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

main()
{
char* identifier;
int i;

pid_t pID = fork();
	for (i = 1;i <= 10;i++)
	{
		
	if(pID==0){
	cout<<i<<endl;
	}
	else if(pID>0)
	{
	cout<<"good\t"<<pID<<endl;

	}
		
	}
}