#include <iostream>
#include <stdio.h>
using namespace std;
 
int f(int *a)
{
int b=5;
a=&b;
}
main()
{
int i;
printf("\n %d",i);
f(&i);
printf("\n %d",i);
}
