#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct process
{
    int number;
    int arrival;
    int prior;
    int burst[100];
    int pointer;
    int max;
    int flag;
};

int preemtive(int n);
int nonpreemtive(int n);
int fcfs(int n);
int round_robin(int n);
int priority(int n);

struct process job[30];

void sort(struct process cpuQ[30],int h,int t)
{
    int j,pos;
    int min=9999;
    for(j=h;j<t;j++){
        if(cpuQ[j].burst[cpuQ[j].pointer]<min &&
cpuQ[j].burst[cpuQ[j].pointer]!=0){
            min=cpuQ[j].burst[cpuQ[j].pointer];
            pos=j;
        }
    }
    struct process temp = cpuQ[h];
    cpuQ[h]=cpuQ[pos];
    cpuQ[pos]=temp;

}

void prio(struct process cpuQ[30],int h,int t)
{
    int j,pos;
    int min=9999;
    for(j=h;j<t;j++){
        if(cpuQ[j].prior<min && cpuQ[j].burst[cpuQ[j].pointer]!=0){
            min=cpuQ[j].prior;
            pos=j;
        }
    }
    struct process temp = cpuQ[h];
    cpuQ[h]=cpuQ[pos];
    cpuQ[pos]=temp;

}

int reading(char *inp)
{
    //char *inname = "test.txt";
    FILE *infile;
    char buffer[50][BUFSIZ]; /* BUFSIZ is defined if you include stdio.h */
    int line_no;
    int length;

    infile = fopen(inp, "r");
    if (!infile)
    {
        printf("Couldn't open file %s for reading.\n", inp);
        return 0;
    }
    char space=' ';
    line_no = 0;

    while (fgets(buffer[line_no], sizeof(buffer[line_no]), infile))
    {
        length = strlen(buffer[line_no]);
        if (buffer[line_no][length - 1] == '\n')
            buffer[line_no][length - 1] = '\0';
        int i=0;int c=0;

        int ar=0;int ap=0;
        while(1)
        {
            if(i==length)
                break;
            if(buffer[line_no][i]==space && ar==1 && ap==1)
            {
                job[line_no].burst[c]=atoi(&buffer[line_no][i+1]);
                c++;
            }
            if(buffer[line_no][i]==space && ar==1 && ap==0)
            {
                job[line_no].prior=atoi(&buffer[line_no][i+1]);
                ap=1;
            }
            if(buffer[line_no][i]==space && ar==0 && ap==0)
            {
                job[line_no].arrival=atoi(&buffer[line_no][i+1]);
                ar=1;
            }
            i++;
        }
        job[line_no].max=c;
        job[line_no].pointer=0;
        job[line_no].number=line_no;
        job[line_no].flag=0;
        /*
        i=0;
        printf("\n%d\t",job[line_no].max );
        printf("%d\t",job[line_no].arrival );
        while(i<c)
        {
            printf("%d\t",job[line_no].cpu[i] );
            i++;
        }
       */
        ++line_no;
    }

    return line_no;
}

int main(void)
{
    int line_no;
    int ch;
    char inp[50];
    printf("%s","Enter file containing input : ");
    scanf("%s",inp);
    do
    {
        printf("%s","\nEnter your choice of job scheduling :
\n1.Pre-emptive\n2.Non Pre-emptive\n3.FCFS\n4.Round-Robin\n5.Priority\n>>>  ");
        scanf("%d",&ch);
        if(ch==1)
        {
            line_no=reading(inp);
            preemtive(line_no);
        }
        else if(ch==2)
        {
            line_no=reading(inp);
            nonpreemtive(line_no);
        }
        else if(ch==3)
        {
            line_no=reading(inp);
            fcfs(line_no);
        }
        else if(ch==4)
        {
            line_no=reading(inp);
            round_robin(line_no);
        }
        else if(ch==5)
        {
            line_no=reading(inp);
            priority(line_no);
        }
        else
            printf("%s","Wrong Choice!!!!\n");
    }while(ch<1 || ch>5);

    printf("\n");
    return 0;
}


int nonpreemtive(int n)
{

    printf("\n%s","\t \t \t \tNon Pre-emptive\n");
    struct process cpuQ[30];
    struct process ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail)
    {
        time++;

        if(cpuhead==-1)
            cpuhead=0;

        for(i=0;i<n;i++)
        {
            if(job[i].arrival==time-1)
            {
                cpuQ[cputail]=job[i];
                cputail++;
            }
        }

        if(cpuhead<cputail)
        {
            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].burst[xyz]=(cpuQ[cpuhead].burst[xyz])-1;
            if(cpuQ[cpuhead].burst[xyz]==0)
            {
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max)
                {
                    cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                    cpuQ[cpuhead].flag=1;
                    ioQ[iotail]=cpuQ[cpuhead];
                    iotail++;
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        sort(cpuQ,cpuhead+1,cputail);
                }
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        sort(cpuQ,cpuhead+1,cputail);
                }
                cpuhead++;
            }
        }

        if(iohead<iotail)
        {
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].burst[xyz]=(ioQ[iohead].burst[xyz])-1;
            if(ioQ[iohead].burst[xyz]==0){
                if(ioQ[iohead].pointer+1<ioQ[iohead].max)
                {
                    ioQ[iohead].pointer++;
                    ioQ[iohead].flag=0;
                    cpuQ[cputail]=ioQ[iohead];
                    cputail++;
                   // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail || (cputail-cpuhead==1 &&
cpuQ[cpuhead].burst[cpuQ[cpuhead].pointer]==0))
               last=time+1;
        }


    }
    printf("\n");
    return 0;
}



int preemtive(int n)
{
    printf("\n%s","\t \t \t \t Pre-emptive\n");
    struct process cpuQ[30];
    struct process ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail)
    {
        time++;
        if(cpuhead==-1)
            cpuhead=0;
        for(i=0;i<n;i++)
        {
            if(job[i].arrival==time-1)
            {
                cpuQ[cputail]=job[i];
                cputail++;
            }
        }

        if(cpuhead<cputail)
        {
            struct process b=cpuQ[cpuhead];
            sort(cpuQ,cpuhead,cputail);
            struct process a=cpuQ[cpuhead];
            if(a.number!=b.number)
            {
                if(last!=time-1)
                    printf("%d : %d   ",b.number+1,time-last-1);
                last=time-1;
            }

            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].burst[xyz]=(cpuQ[cpuhead].burst[xyz])-1;

            if(cpuQ[cpuhead].burst[xyz]==0)
            {
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max)
                {
                    cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                    cpuQ[cpuhead].flag=1;
                    ioQ[iotail]=cpuQ[cpuhead];
                    iotail++;
                    if(cpuQ[cpuhead].number==ioQ[iohead].number)
                         ioQ[iohead].burst[ioQ[iohead].pointer]++;
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        sort(cpuQ,cpuhead+1,cputail);
                }
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        sort(cpuQ,cpuhead+1,cputail);
                }

                cpuhead++;
            }
        }

        if(iohead<iotail)
        {
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].burst[xyz]=(ioQ[iohead].burst[xyz])-1;

            if(ioQ[iohead].burst[xyz]==0)
            {
                if(ioQ[iohead].pointer+1<ioQ[iohead].max)
                {
                    ioQ[iohead].pointer++;
                    ioQ[iohead].flag=0;
                    cpuQ[cputail]=ioQ[iohead];
                    cputail++;
                   // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail || (cputail-cpuhead==1 &&
cpuQ[cpuhead].burst[cpuQ[cpuhead].pointer]==0))
              last=time+1;
        }

    }

    printf("\n");
    return 0;
}

int fcfs(int n){


    printf("\n%s","\t \t \t \tFCFS\n");
    struct process cpuQ[30];
    struct process ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail)
    {
        time++;

        if(cpuhead==-1)
            cpuhead=0;

        for(i=0;i<n;i++)
        {
            if(job[i].arrival==time-1)
            {
                cpuQ[cputail]=job[i];
                cputail++;
            }
        }

        if(cpuhead<cputail)
        {
            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].burst[xyz]=(cpuQ[cpuhead].burst[xyz])-1;
            if(cpuQ[cpuhead].burst[xyz]==0)
            {
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max)
                {
                    cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                    cpuQ[cpuhead].flag=1;
                    ioQ[iotail]=cpuQ[cpuhead];
                    iotail++;
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                }
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                }
                cpuhead++;
            }
        }

        if(iohead<iotail)
        {
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].burst[xyz]=(ioQ[iohead].burst[xyz])-1;
            if(ioQ[iohead].burst[xyz]==0){
                if(ioQ[iohead].pointer+1<ioQ[iohead].max)
                {
                    ioQ[iohead].pointer++;
                    ioQ[iohead].flag=0;
                    cpuQ[cputail]=ioQ[iohead];
                    cputail++;
                   // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail || (cputail-cpuhead==1 &&
cpuQ[cpuhead].burst[cpuQ[cpuhead].pointer]==0))
               last=time+1;
        }


    }
    printf("\n");
    return 0;

}

int round_robin(int n){


    printf("\n%s","\t \t \t \tRound-Robin\n");
    struct process cpuQ[30];
    struct process ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;
    int quantum,h,slice;
    scanf("%d",&quantum);
    slice=quantum;

    while(cpuhead<cputail || iohead<iotail)
    {
        time++;

        if(cpuhead==-1)
            cpuhead=0;

        for(i=0;i<n;i++)
        {
            if(job[i].arrival==time-1)
            {
                cpuQ[cputail]=job[i];
                cputail++;
            }
        }

        if(cpuhead<cputail)
        {
            if(slice==0){
                slice=quantum;
                h=cpuhead;
                struct process temp = cpuQ[h];
                printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last-1);
                    last=time-1;
                for(i=h;i<cputail-1;i++){
                    cpuQ[i]=cpuQ[i+1];
                }
                cpuQ[i]=temp;
            }
            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].burst[xyz]=(cpuQ[cpuhead].burst[xyz])-1;
            slice--;
            if(cpuQ[cpuhead].burst[xyz]==0)
            {
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max)
                {
                    cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                    cpuQ[cpuhead].flag=1;
                    ioQ[iotail]=cpuQ[cpuhead];
                    iotail++;
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                }
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                }
                cpuhead++;
                slice=quantum;
            }
        }


        if(iohead<iotail)
        {
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].burst[xyz]=(ioQ[iohead].burst[xyz])-1;
            if(ioQ[iohead].burst[xyz]==0){
                if(ioQ[iohead].pointer+1<ioQ[iohead].max)
                {
                    ioQ[iohead].pointer++;
                    ioQ[iohead].flag=0;
                    cpuQ[cputail]=ioQ[iohead];
                    cputail++;
                   // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail || (cputail-cpuhead==1 &&
cpuQ[cpuhead].burst[cpuQ[cpuhead].pointer]==0))
               last=time+1;
        }


    }
    printf("\n");
    return 0;

}

int priority(int n)
{
    printf("\n%s","\t \t \t \t Priority\n");
    struct process cpuQ[30];
    struct process ioQ[30];
    int xyz,time=0;
    int cpuhead,cputail,iohead,iotail;
    cpuhead=iohead=cputail=iotail=0;
    int i,last=0;
    cpuhead=-1;

    while(cpuhead<cputail || iohead<iotail)
    {
        time++;
        if(cpuhead==-1)
            cpuhead=0;
        for(i=0;i<n;i++)
        {
            if(job[i].arrival==time-1)
            {
                cpuQ[cputail]=job[i];
                cputail++;
            }
        }

        if(cpuhead<cputail)
        {
            struct process b=cpuQ[cpuhead];
            prio(cpuQ,cpuhead,cputail);
            struct process a=cpuQ[cpuhead];
            if(a.number!=b.number)
            {
                if(last!=time-1)
                    printf("%d : %d   ",b.number+1,time-last-1);
                last=time-1;
            }

            xyz=cpuQ[cpuhead].pointer;
            cpuQ[cpuhead].burst[xyz]=(cpuQ[cpuhead].burst[xyz])-1;

            if(cpuQ[cpuhead].burst[xyz]==0)
            {
                if(cpuQ[cpuhead].pointer+1<cpuQ[cpuhead].max)
                {
                    cpuQ[cpuhead].pointer=(cpuQ[cpuhead].pointer)+1;
                    cpuQ[cpuhead].flag=1;
                    ioQ[iotail]=cpuQ[cpuhead];
                    iotail++;
                    if(cpuQ[cpuhead].number==ioQ[iohead].number)
                         ioQ[iohead].burst[ioQ[iohead].pointer]++;
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        prio(cpuQ,cpuhead+1,cputail);
                }
                if(cpuQ[cpuhead].pointer+1==cpuQ[cpuhead].max)
                {
                    if(time!=last)
                        printf("%d : %d   ",cpuQ[cpuhead].number+1,time-last);
                    last=time;
                    if((cpuhead+1)!=cputail)
                        prio(cpuQ,cpuhead+1,cputail);
                }

                cpuhead++;
            }
        }

        if(iohead<iotail)
        {
            xyz=ioQ[iohead].pointer;
            ioQ[iohead].burst[xyz]=(ioQ[iohead].burst[xyz])-1;

            if(ioQ[iohead].burst[xyz]==0)
            {
                if(ioQ[iohead].pointer+1<ioQ[iohead].max)
                {
                    ioQ[iohead].pointer++;
                    ioQ[iohead].flag=0;
                    cpuQ[cputail]=ioQ[iohead];
                    cputail++;
                   // printf("\t %d : %d ",cpuQ[cpuhead].number+1,time-last+1);
                }

                iohead++;
            }
            if(cpuhead>=cputail || (cputail-cpuhead==1 &&
cpuQ[cpuhead].burst[cpuQ[cpuhead].pointer]==0))
              last=time+1;
        }

    }

    printf("\n");
    return 0;
}
