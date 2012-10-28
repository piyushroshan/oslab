#include <iostream>
#include <stdio.h>
using namespace std;
void f(int i)
{
int j;
for (j=0;j<16;j++)
{
if (i & (0x8000>>j))
printf("1");
else
printf("0");
}
}
int main()
{
int a;
cin>>a;
f(a);
return 0;
}
