#include <cstdlib>
#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

HANDLE h;
int q_index;
int n;


typedef enum JOBSTATUS{DONE,WAITING,RUNNING};

typedef struct job
{
        int jobid;
        int time_complete;
        int wait_time;
        int turnaround;
        int status;
        int time_arrive;
};


void gotoxy(int,int);
void setcolor(int);
void do_preemptive();
void do_nonpreemptive();
void swap(job*,job*);
void sort_by_shortest(job*,int);


using namespace std;

int main(int argc, char *argv[])
{
	h = GetStdHandle(STD_OUTPUT_HANDLE);
    system("cls");
    do_nonpreemptive();    
    getch();
    system("cls");
    do_preemptive();
    system("PAUSE");
    return EXIT_SUCCESS;
}


void gotoxy(int x,int y)
{
     COORD c;
     c.X = x;
     c.Y = y;
     SetConsoleCursorPosition(h,c);
}

void setcolor(int c)
{
     SetConsoleTextAttribute(h,c);
}


void swap(job *a,job *b)
{
     job tmp;

     tmp.jobid = a->jobid;     
     tmp.time_complete = a->time_complete;     
     tmp.wait_time = a->wait_time;     
     tmp.turnaround = a->turnaround;     
     tmp.status = a->status;     
     tmp.time_arrive = a->time_arrive;                              

     a->jobid = b->jobid;     
     a->time_complete=b->time_complete;     
     a->wait_time = b->wait_time;     
     a->turnaround = b->turnaround;     
     a->status = b->status;     
     a->time_arrive = b->time_arrive; 

     b->jobid = tmp.jobid;     
     b->time_complete=tmp.time_complete;     
     b->wait_time = tmp.wait_time;     
     b->turnaround = tmp.turnaround;     
     b->status = tmp.status;     
     b->time_arrive = tmp.time_arrive;      
}

void sort_by_shortest(job *j,int n)
{
     int i,k,min;
     
     for(k=0;k<n-1;k++)
     {
        min = k;
        
        for(i=k;i<n;i++)
        {
            if (j[i].time_complete < j[min].time_complete)
            {
                min = i;
            }
        }               
        
        if (k != min)
        {
             swap(&j[k],&j[min]);
        }        
     }
}

void print_jobs(job *jobs)
{
    for (int i=0;i<n;i++)
    {
        printf("\nJobID: %4d,  Complete: %4d, Wait: %4d,  TA: %4d,  Status: %d,  Arrive: %d",jobs[i].jobid,jobs[i].time_complete,jobs[i].wait_time,jobs[i].turnaround,jobs[i].status,jobs[i].time_arrive);
    }   
}

void print_jobs2(job *jobs)
{
    for (int i=0;i<=q_index;i++)
    {
        printf("\nJobID: %4d,  Complete: %4d, Wait: %4d,  TA: %4d,  Status: %d,  Arrive: %d",jobs[i].jobid,jobs[i].time_complete,jobs[i].wait_time,jobs[i].turnaround,jobs[i].status,jobs[i].time_arrive);
    }   
}


void do_nonpreemptive()
{
    float awt,atat;
    int time=0;
    int cid;
    job jobs[10]={{1,5,0,0,1},
                  {2,25,0,0,1},
                  {3,4,0,0,1},
                  {4,11,0,0,1},
                  {5,13,0,0,1},
                  {6,8,0,0,1},
                  {7,33,0,0,1},
                  {8,2,0,0,1},
                  {9,19,0,0,1},
                  {10,21,0,0,1},
                 };
    n=10;
    printf("\nNON-PREEMPTIVE SHORTEST JOB FIRST");
    print_jobs(jobs);
    sort_by_shortest(jobs,10);
    printf("\nHit any key to continue...");
    getch();

    for (cid=0;cid<10;cid++)
    {
        jobs[cid].status = RUNNING;
        time = 0;

        while (jobs[cid].time_complete > 0)
        {
              gotoxy(1,12);
              print_jobs(jobs);
              jobs[cid].time_complete--;
              for(int k=0;k<n;k++)
              {
                      if (jobs[k].status == WAITING)
                         jobs[k].wait_time++;
                         
                      if (jobs[k].status != DONE)
                         jobs[k].turnaround++;
              }
              Sleep(100);
              time++;              
        }
        jobs[cid].status = DONE;
    }
    gotoxy(1,12);
    print_jobs(jobs);

    awt = 0.0f;
    atat = 0.0f;
    for (int i=0;i<n;i++)
    {
       awt = awt + (float) jobs[i].wait_time;
       atat = atat + (float) jobs[i].turnaround;
    }
    printf("\n\nAverage wait time: %4.2f\nAverage turnaround time: %4.2f\n",awt/10,atat/10);
}



void q_job(job *q,job j)
{
     q_index++;     
     q[q_index].jobid = j.jobid;
     q[q_index].time_complete = j.time_complete;
     q[q_index].wait_time = j.wait_time;
     q[q_index].turnaround = j.turnaround;
     q[q_index].status =j.status;
     q[q_index].time_arrive=j.time_arrive;
}


void do_preemptive()
{
    float awt,atat;
    int time;
    int cid;
    int notdone=1;
    int sum_status;
    job jobs[10]={{1,12,0,0,1,0},
                  {2,4,0,0,1,2},
                  {3,8,0,0,1,5},
                  {4,5,0,0,1,7},
                 };
    n = 4;
    job queue[10];                 

    printf("\nPREEMPTIVE SHORTEST JOB NEXT");
    print_jobs(jobs);
    printf("\nHit any key to continue...");
    getch();

    time = 0;
    q_index = -1;

    q_job(queue,jobs[0]);
    queue[q_index].status = RUNNING;
        
    do
    {
        gotoxy(1,11);
        printf("Time: %d",time);
        gotoxy(1,12);
        print_jobs2(queue);

        //check if a new job has arrived
        for(int k=1;k<n;k++)
        {
             if (jobs[k].time_arrive == time)
             {
                 gotoxy(1,12);
                 print_jobs2(queue);
                 if (queue[0].time_complete > jobs[k].time_complete)
                 {
                    queue[0].status = WAITING;
                    jobs[k].status = RUNNING;                    
                 }
                 q_job(queue,jobs[k]);
                 sort_by_shortest(queue,q_index+1);
             }
        }
        notdone=0;
        
        for (int k=0;k<=q_index;k++)
        {
             notdone += queue[k].status;
             if (queue[k].status == WAITING)
                queue[k].wait_time++;
                         
             if (queue[k].status != DONE)
                queue[k].turnaround++;
        }
        queue[0].time_complete--;

        if (queue[0].time_complete==0)
        {
           queue[0].time_complete = 9999;
           queue[0].status = DONE;

           if (queue[1].status != DONE)
              queue[1].status = RUNNING;

           sort_by_shortest(queue,q_index+1);
        }
        Sleep(800);
        time++;              
    }while (notdone);
    
    awt = 0.0f;
    atat = 0.0f;
    for (int i=0;i<=q_index;i++)
    {
       awt = awt + (float) queue[i].wait_time;
       atat = atat + (float) queue[i].turnaround;
    }
    printf("\n\nAverage wait time: %4.2f\nAverage turnaround time: %4.2f\n",awt/(q_index+1),atat/(q_index+1));
}
