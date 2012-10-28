#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
typedef struct proc
{
    int number;
    int arrival;
    int cpu[100];
    int pointer;
    int max;
    int flag;
};

int preemtive(int n);
int nonpreemtive(int n);
struct proc process[30];

void sort(struct proc cpuQ[30],int h,int t){
    int j,pos;
    int min=9999;
    for(j=h;j<t;j++){
        if(cpuQ[j].cpu[cpuQ[j].pointer]<min){
            min=cpuQ[j].cpu[cpuQ[j].pointer];
            pos=j;
        }
    }
    struct proc temp = cpuQ[h];
    cpuQ[h]=cpuQ[pos];
    cpuQ[pos]=temp;

}

int main(void)
{
    char *inname = "test.txt";
    FILE *infile;
    char line_buffer[50][BUFSIZ]; /* BUFSIZ is defined if you include stdio.h */
    char line_number;
    int length;

    infile = fopen(inname, "r");
    if (!infile) {
        printf("Couldn't open file %s for reading.\n", inname);
        return 0;
    }
    char space=' ';
    printf("Opened file %s for reading.\n", inname);

    line_number = 0;
    //struct proc process[30];
    while (fgets(line_buffer[line_number], sizeof(line_buffer[line_number]), infile)) {


        /* note that the newline is in the buffer */
        length = strlen(line_buffer[line_number]);
        if (line_buffer[line_number][length - 1] == '\n')
            line_buffer[line_number][length - 1] = '\0';
        printf("%4d: %s\n", line_number, line_buffer[line_number]);
        int i=0;int c=0;

        {
            /* data */
        };
        int ar=0;
        while(1)
        {
            if(i==length)
                break;
            if(line_buffer[line_number][i]==space && ar==1)
            {
                process[line_number].cpu[c]=atoi(&line_buffer[line_number][i+1]);
                c++;
            }
            if(line_buffer[line_number][i]==space && ar==0)
            {
                process[line_number].arrival=atoi(&line_buffer[line_number][i+1]);
                ar=1;
            }
            i++;
        }
        process[line_number].max=c;
        process[line_number].pointer=0;
        process[line_number].number=line_number;
        process[line_number].flag=0;
        /*
        i=0;
        printf("\n%d\t",process[line_number].max );
        printf("%d\t",process[line_number].arrival );
        while(i<c)
        {
            printf("%d\t",process[line_number].cpu[i] );
            i++;
        }
       */
        ++line_number;
    }
nonpreemtive(line_number);

    printf("\nTotal number of lines = %d\n", line_number);
    preemtive(line_number);

    return 0;
}


int preemtive(int n)
{

    struct proc cpuQ[30];
    struct proc ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail){

time++;

        if(cpuhead==-1)
            cpuhead=0;
        for(i=0;i<n;i++){
            if(process[i].arrival==time-1){
                cpuQ[cputail]=process[i];
                cputail++;
            }
        }

        if(cpuhead<cputail){
            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].cpu[xyz]=(cpuQ[cpuhead].cpu[xyz])-1;
            if(cpuQ[cpuhead].cpu[xyz]==0){
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max){
                cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                cpuQ[cpuhead].flag=1;
                ioQ[iotail]=cpuQ[cpuhead];
                iotail++;
                printf("  %d : %d   ",cpuQ[cpuhead].number+1,time-last);
               last=time;
                if((cpuhead+1)!=cputail)
                    sort(cpuQ,cpuhead+1,cputail);}
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                printf("  %d : %d   ",cpuQ[cpuhead].number+1,time-last);
               last=time;
                if((cpuhead+1)!=cputail)
                    sort(cpuQ,cpuhead+1,cputail);}
                cpuhead++;
            }
        }

        if(iohead<iotail){
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].cpu[xyz]=(ioQ[iohead].cpu[xyz])-1;
            if(ioQ[iohead].cpu[xyz]==0){
                if(ioQ[iohead].pointer+1<ioQ[iohead].max){
                ioQ[iohead].pointer++;
                ioQ[iohead].flag=1;
                cpuQ[cputail]=ioQ[iohead];
                cputail++;
               // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail)
               last=time+1;
        }


    }

    return 0;
}



int nonpreemtive(int n)
{

    struct proc cpuQ[30];
    struct proc ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail){
        time++;
        if(cpuhead==-1)
            cpuhead=0;
        for(i=0;i<n;i++){
            if(process[i].arrival==time-1){
                cpuQ[cputail]=process[i];
                cputail++;
            }
        }

        if(cpuhead<cputail){
            struct proc b=cpuQ[cpuhead];
            sort(cpuQ,cpuhead,cputail);
            struct proc a=cpuQ[cpuhead];
            if(a.number!=b.number){
                printf("  %d : %d   ",b.number+1,time-last-1);
               last=time-1;
            }

            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].cpu[xyz]=(cpuQ[cpuhead].cpu[xyz])-1;
            if(cpuQ[cpuhead].cpu[xyz]==0){
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max){
                cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                cpuQ[cpuhead].flag=1;
                ioQ[iotail]=cpuQ[cpuhead];
                iotail++;
                printf("  %d : %d   ",cpuQ[cpuhead].number+1,time-last);
               last=time;
                if((cpuhead+1)!=cputail)
                    sort(cpuQ,cpuhead+1,cputail);}
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                printf("  %d : %d   ",cpuQ[cpuhead].number+1,time-last);
               last=time;
                if((cpuhead+1)!=cputail)
                    sort(cpuQ,cpuhead+1,cputail);}
                cpuhead++;
            }
        }

        if(iohead<iotail){
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].cpu[xyz]=(ioQ[iohead].cpu[xyz])-1;
            if(ioQ[iohead].cpu[xyz]==0){
                if(ioQ[iohead].pointer+1<ioQ[iohead].max){
                ioQ[iohead].pointer++;
                ioQ[iohead].flag=1;
                cpuQ[cputail]=ioQ[iohead];
                cputail++;
               // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail)
              last=time+1;
        }

    }

    return 0;
}
