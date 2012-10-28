#include <pthread.h>
#include <stdio.h>
char a[100];
void* Vowels()
{
    int va=0,ve=0,vi=0,vo=0,vu=0,i;
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='a' || a[i]=='A')
            va++;
    }
    printf("No of a --> %d\n", va);
    usleep(1000);
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='e' || a[i]=='E')
            ve++;
     }
    printf("No of e --> %d\n", ve);
    usleep(1000);
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='i' || a[i]=='I')
            vi++;
     }
    printf("No of i --> %d\n", vi);
    usleep(1000);
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='o' || a[i]=='O')
            vo++;
     }
    printf("No of o --> %d\n", vo);
    usleep(1000);
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='u' || a[i]=='U')
            vu++;
    }
    printf("No of u --> %d\n", vu);
    usleep(1000);
}

void *Words()
{
   int i=0;
   char words[100];
    for (i=0;a[i]!='\0';i++) {
        int j=0;
        while(a[i]!=' ' &&  a[i]!='\0')
        {
            words[j++]=a[i];
            i++;
        }
        words[j]='\0';

        printf("Word found --> %s\n", words);
  		usleep(1000);
    }
}



int main()
{
pthread_t inc_thread[2];
gets(a);
if(pthread_create(&inc_thread[0], NULL, Vowels,NULL)) {
fprintf(stderr, "Error creating thread\n");
return 1;
}

if(pthread_create(&inc_thread[1], NULL, Words,NULL)) {
fprintf(stderr, "Error creating thread\n");
return 1;
}

if(pthread_join(inc_thread[0], NULL)) {
fprintf(stderr, "Error joining thread\n");
return 2;
}

if(pthread_join(inc_thread[1], NULL)) {
fprintf(stderr, "Error joining thread\n");
return 2;
}
return 0;

}
