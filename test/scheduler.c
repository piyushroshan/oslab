#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
# define MAXSIZE 100
# define MAXPROC 10
int qp[MAXSIZE];
int q[MAXSIZE];
int front, rear;
int frontp, rearp;
int line_number;
struct proc
{
    int number;
    int arrival;
    int cpu[100];
    int pointer;
    int max;
    bool cpuf;
};

void del()
{
    int a;
    if(front == rear)
    {
        printf("QUEUE EMPTY");
        return;
    }
    else
    {
        a=q[front];
        q[front]=-2;
        ++front;
    }
    return;
}

void add(int a)
{
    if(rear>MAXSIZE)
    {
        printf("QUEUE FULL");
        return;
    }
    else
    {
        q[rear]=a;
        rear++;
        //printf("rear = %d front = %d",rear,front);
    }
}


void delp()
{
    int a;
    if(frontp == rearp)
    {
        printf("QUEUEP EMPTY");
        return;
    }
    else
    {
        a=qp[frontp];
        qp[frontp]=-2;
        ++frontp;
    }
    return;
}

void addp(int a)
{
    if(rearp>MAXSIZE)
    {
        printf("QUEUEP FULL");
        return;
    }
    else
    {
        qp[rearp]=a;
        rearp++;
        //printf("rearp = %d frontp = %d",rearp,frontp);
    }
}

int nextp()
{
    int a;
    if(frontp < rearp-1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



bool qp_empty()
{
    if(frontp == rearp)
        return true;
    else
        return false;
}
bool q_empty()
{
    if(front == rear)
        return true;
    else
        return false;
}

bool finish(struct proc process[MAXPROC])
{
    int i;
    bool ret;
    ret=false;
    for ( i = 0; i < line_number; ++i)
    {
        if(process[i].pointer < process[i].max)
            ret = true;
    }
    return ret;
}

int main(void)
{
    int preemtive(int line_number,struct proc process[MAXPROC]);
    int nonpreemtive(int line_number,struct proc process[MAXPROC]);
    int FCFS(int line_number,struct proc process[MAXPROC]);
    int RoundRobin(int line_number,struct proc process[MAXPROC]);
    int choice;
    int loop=4;
    while(1)
    {
        int i;
        for(i=0; i<MAXSIZE; i++)
            q[i]=-2;
        char *inname = "test.txt";
        FILE *infile;
        char line_buffer[50][BUFSIZ]; /* BUFSIZ is defined if you include stdio.h */
        int length;

        infile = fopen(inname, "r");
        if (!infile)
        {
            printf("Couldn't open file %s for reading.\n", inname);
            return 0;
        }
        char space=' ';
        printf("Opened file %s for reading.\n", inname);

        line_number = 0;
        struct proc process[MAXPROC];
        while (fgets(line_buffer[line_number], sizeof(line_buffer[line_number]), infile))
        {


            /* note that the newline is in the buffer */
            length = strlen(line_buffer[line_number]);
            if (line_buffer[line_number][length - 1] == '\n')
                line_buffer[line_number][length - 1] = '\0';
            printf("%4d: %s\n", line_number, line_buffer[line_number]);
            int i=0;
            int c=0;

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
            process[line_number].number=atoi(&line_buffer[line_number][0]);
            process[line_number].cpuf=true;

            ++line_number;
        }
        fclose(infile);
        printf("1. Pre-emptive\n");
        printf("2. Non-emptive\n");
        printf("3. FCFS\n");
        printf("4. Round Robin\n");
        printf("5. To exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            preemtive(line_number,process);
            break;
        case 2:
            nonpreemtive(line_number,process);
            break;
        case 3:
            FCFS(line_number,process);
            break;
        case 4:
            RoundRobin(line_number,process);
            break;
        case 5:
            return 0;
        default:
            printf("Wrong Choice\n");
        }
    }
    printf("\nTotal number of lines = %d\n", line_number);

    return 0;
}


int preemtive(int line_number,struct proc process[MAXPROC])
{
    front =0;
    rear = 0;
    frontp =0;
    rearp = 0;
    int time = 0;
    int i;
    int exec_no,exec_nop;

    int count=0,countp=0;
    int ariv=9999;
    int min=9999;
    int ariv_p=-1;
    printf("Preemtive-------------------------------------\n");
    for( i=0; i<line_number; i++)
    {
        if(process[i].arrival<ariv)
        {
            ariv=process[i].arrival;
            ariv_p=i;
        }

    }
    exec_no = ariv_p;
    exec_nop = exec_no;
    time=time+ariv;

    min=process[ariv_p].cpu[process[ariv_p].pointer];
    while(1)
    {


        min=9999;
        for(i=0; i<line_number; i++)
        {
            if(process[i].arrival<=time)
            {
                if(process[i].cpuf==true)
                {
                    if((process[i].pointer < process[i].max) && (process[i].cpu[process[i].pointer] < min))
                    {
                        min = process[i].cpu[process[i].pointer];

                        exec_no = i;
                    }
                }
            }
        }
        if(exec_no!=exec_nop &&exec_nop!=-1)
        {
            printf("P%d\t: %d\n",process[exec_nop].number,count );
            //printf("min: %d\n",min);
            count=0;
            exec_nop=exec_no;
        }
        else
        {
            countp=count;
            exec_nop=exec_no;
        }
        if(!finish(process))
        {
            break;
        }
        if(exec_no==-1)
        {
            if ((q[front] >= 0) && process[q[front]].cpu[process[q[front]].pointer] == 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpuf = true;
                //printf("io%d",time);

                process[q[front]].pointer++;
                del();
            }
            if((q[front] >= 0) && process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;

                if (process[q[front]].cpu[process[q[front]].pointer] == 0)
                {
                    process[q[front]].cpuf = true;
                    //printf("io%d",time);

                    process[q[front]].pointer++;
                    del();
                }
            }
        }



        if (exec_no!=-1 && min != 0 )
        {
            if ((q[front] >= 0) && process[q[front]].cpu[process[q[front]].pointer] == 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpuf = true;
                //printf("io%d",time);

                process[q[front]].pointer++;
                del();
            }
            if((q[front] >= 0) && process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
                if (process[q[front]].cpu[process[q[front]].pointer]==0)
                {
                    process[q[front]].cpuf = true;
                    //printf("io%d",time);

                    process[q[front]].pointer++;
                    del();
                }
            }



            if (process[exec_no].cpu[process[exec_no].pointer]>0 && process[exec_no].cpuf == true)
            {
                process[exec_no].cpu[process[exec_no].pointer]--;
                count++;

                if(process[exec_no].cpu[process[exec_no].pointer]==0)
                {
                    add(exec_no);
                    process[exec_no].pointer++;
                    process[exec_no].cpuf = false;
                    exec_no = -1;
                }
            }
        }

        time++;

    }
    return 0;
}

int FCFS(int line_number,struct proc process[MAXPROC])
{
    front =0;
    rear = 0;
    frontp =0;
    rearp = 0;
    int time = 0;
    int i,j,k,flag;
    int itime = 0;
    bool run;
    int exec_no;
    int ariv=9999;
    int min=9999;
    int ariv_p=-1;
    printf("FCFS-------------------------------------\n");
    int arrival[line_number];
    for (i = 0; i < line_number; i++)
    {
        arrival[i]=9999;
    }
    for( i=0; i<line_number; i++)
    {
        for( j=0; j<line_number; j++)
        {
            if(process[j].arrival < ariv)
            {
                flag = 1;
                for(k=0; k<i; k++)
                {
                    if (j==arrival[k])
                        flag=0;
                }
                if (flag)
                {
                    ariv=process[j].arrival;
                    ariv_p=j;
                }

            }
        }
        arrival[i]=ariv_p;
        ariv=9999;
    }

    for (i = 0; i < line_number; i++)
    {
        printf("%d\t",arrival[i] );
    }
    exec_no = arrival[0];
    int plast=0;
    time=time+process[exec_no].arrival;
    min = process[exec_no].cpu[process[exec_no].pointer];
    while(finish(process))
    {
        if(time!=ariv)
        {
            //min=9999;
            for(i=0; i<line_number; i++)
            {
                if(process[arrival[(i+plast)%line_number]].arrival<=time)
                {
                    if(process[arrival[(i+plast)%line_number]].cpuf==true)
                    {
                        if(process[arrival[(i+plast)%line_number]].pointer < process[arrival[(i+plast)%line_number]].max)
                        {
                            min = process[arrival[(i+plast)%line_number]].cpu[process[arrival[(i+plast)%line_number]].pointer];

                            exec_no = arrival[(i+plast)%line_number];
                            break;
                        }
                    }
                }
            }
        }
        if (exec_no==-1)
        {
            run = false;
        }
        if (exec_no!=-1 && min!=0)
        {
            printf("P%d\t: %d\n",process[exec_no].number,min );
        }

        while(process[exec_no].cpu[process[exec_no].pointer] > 0 && (exec_no!=-1))
        {
            time++;
            itime++;
            if((front != rear) && process[q[front]].cpu[process[q[front]].pointer] > 0 && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((front != rear) && (q[front] >=0) && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);

                process[q[front]].pointer++;
                del();
            }
            process[exec_no].cpu[process[exec_no].pointer]--;
        }
        if(exec_no==-1)
        {
            time++;
            itime++;

            if(process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((q[front] >=0) && front != rear && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);
                process[q[front]].pointer++;
                del();

            }
        }

        if(exec_no!=-1)
        {
            add(exec_no);
            plast=(exec_no+1)%line_number;
            process[exec_no].pointer++;
            process[exec_no].cpuf = false;
            exec_no = -1;
        }
    }
    return 0;
}


int nonpreemtive(int line_number,struct proc process[MAXPROC])
{
    front =0;
    rear = 0;
    frontp =0;
    rearp = 0;
    int time = 0;
    int i;
    int itime = 0;
    bool run;
    int exec_no;
    int ariv=9999;
    int min=9999;
    int ariv_p=-1;
    printf("NON PREMPTIVE-------------------------------------\n");
    for( i=0; i<line_number; i++)
    {
        if(process[i].arrival<ariv)
        {
            ariv=process[i].arrival;
            ariv_p=i;
        }

    }
    exec_no = ariv_p;
    time=time+ariv;

    min=process[ariv_p].cpu[process[ariv_p].pointer];
    while(finish(process))
    {
        if(time!=ariv)
        {
            min=9999;
            for(i=0; i<line_number; i++)
            {
                if(process[i].arrival<=time)
                {
                    if(process[i].cpuf==true)
                    {
                        if((process[i].pointer < process[i].max) && (process[i].cpu[process[i].pointer] < min))
                        {
                            min = process[i].cpu[process[i].pointer];

                            exec_no = i;
                        }
                    }
                }
            }
        }

        if (exec_no==-1)
        {
            run = false;
        }
        if (exec_no!=-1 && min!=0)
        {
            printf("P%d\t: %d\n",process[exec_no].number,min );
        }

        while(process[exec_no].cpu[process[exec_no].pointer] > 0 && (exec_no!=-1))
        {
            time++;
            itime++;
            if(process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((q[front] >=0) && front != rear && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);

                process[q[front]].pointer++;
                del();
            }
            process[exec_no].cpu[process[exec_no].pointer]--;
        }
        if(exec_no==-1)
        {
            time++;
            itime++;

            if(process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((q[front] >=0) && front != rear && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);
                process[q[front]].pointer++;
                del();

            }
        }

        if(exec_no!=-1)
        {
            add(exec_no);
            process[exec_no].pointer++;
            process[exec_no].cpuf = false;
            exec_no = -1;
        }
    }
    return 0;
}


int RoundRobin(int line_number,struct proc process[MAXPROC])
{
    front =0;
    rear = 0;
    frontp =0;
    rearp = 0;
    int time = 0;
    int i,j,k,flag;
    int itime = 0;
    bool run;
    int exec_no;
    int quantum=0;
    printf("Enter quantun \n:");
    scanf("%d",&quantum);
    int ariv=9999;
    int min=9999;
    int ariv_p=-1;
    printf("RoundRobin-------------------------------------\n");
    int arrival[line_number];
    for (i = 0; i < MAXSIZE; i++)
    	qp[i]=-2;
    {
    	/* code */
    }
    for (i = 0; i < line_number; i++)
    {
        arrival[i]=9999;
    }
    for( i=0; i<line_number; i++)
    {
        for( j=0; j<line_number; j++)
        {
            if(process[j].arrival < ariv)
            {
                flag = 1;
                for(k=0; k<i; k++)
                {
                    if (j==arrival[k])
                        flag=0;
                }
                if (flag)
                {
                    ariv=process[j].arrival;
                    ariv_p=j;
                }

            }
        }
        arrival[i]=ariv_p;
        ariv=9999;

    }

    for (i = 0; i < line_number; i++)
    {
        printf("%d\t",arrival[i] );
    }
    exec_no = arrival[0];
    int plast=0;
    time=0;
    min = process[exec_no].cpu[process[exec_no].pointer];
    while(finish(process))
    {

            //min=9999;
            for (i = 0; i < line_number; i++)
            {
            	if(time==process[i].arrival)
            	if(frontp==rearp)
            		addp(i);
                else
                if(qp[rearp-1]!=i)
                  add(i);
            }
            i=frontp+1; j=frontp;

                if(nextp())
                {
                 if(process[qp[i]].arrival<=time)
                {
                	int temp = qp[i];
                    if(process[qp[i]].cpuf==true)
                    {
                        if(process[qp[i]].pointer < process[qp[i]].max)
                        {
                            min = process[qp[i]].cpu[process[qp[i]].pointer];
                            if(min>0)
                            {
                            exec_no = qp[i];
                            if(!qp_empty())
                                delp();
                            addp(qp[j]);
                            break;
                        	}
                        }
                    }
                }
                }else{
                exec_no = qp[frontp];
                min = process[qp[frontp]].cpu[process[qp[frontp]].pointer];
                }
        //printf("%d\n", exec_no);
        if (exec_no==-1)
        {
            run = false;
        }
        itime=0;

        while(process[exec_no].cpu[process[exec_no].pointer] > 0 && (exec_no!=-1))
        {
            time++;
            for (i = 0; i < line_number; i++)
            {
            	if(time==process[i].arrival)
            		addp(i);
            }
            if((front != rear) && process[q[front]].cpu[process[q[front]].pointer] > 0 && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((front != rear) && (q[front] >=0) && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);

                process[q[front]].pointer++;
                del();
            }
            process[exec_no].cpu[process[exec_no].pointer]--;
            itime++;
            if (itime>=quantum)
            {
            	break;
            }
        }
        if(exec_no==-1)
        {
            time++;

            if(process[q[front]].cpu[process[q[front]].pointer] > 0 && (front != rear) && (process[q[front]].cpuf == false))
            {
                process[q[front]].cpu[process[q[front]].pointer]--;
            }
            else if((q[front] >=0) && front != rear && (process[q[front]].cpuf == false))
            {

                process[q[front]].cpuf = true;
                //printf("io%d",time);
                process[q[front]].pointer++;
                del();

            }
            itime++;
            if (itime>=quantum)
            {
            	break;
            }
        }

        if (exec_no!=-1 && min!=0)
        {
            printf("P%d\t: %d\t %d\n",process[exec_no].number,min,itime );
        }

        if(exec_no!=-1 && (process[exec_no].cpu[process[exec_no].pointer] <= 0))
        {
            add(exec_no);
            plast=exec_no;
            process[exec_no].pointer++;
            process[exec_no].cpuf = false;
            exec_no = -1;
        }
    }
    return 0;
}
