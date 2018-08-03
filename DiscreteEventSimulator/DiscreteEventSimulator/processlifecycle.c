/*
*Naimul Khan
*tuf70149@temple.edu
*3207
*Program Description: This program mimics the lifecycle of a process using FIFO queues and priority queues for each componoent
*/



#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// struct event initialized
struct event{
    int execTime;
    int eventState;
    int id;
};

// function prototypes
void add(struct event, struct event[]);
void addToCPU(struct event, struct event[]);
struct event deleteFromCPU();
void addToDisk1(struct event, struct event[]);
struct event deleteFromDisk1();
void addToDisk2(struct event, struct event[]);
struct event deleteFromDisk2();
struct event delete();
void bubbleSort(struct event[]);
int size(struct event[], int, int);
bool isEmpty();
void setup();
void JOB_ARRIVAL_EVENT(struct event);
void CPU_HANDLER(struct event);
void DISK1_HANDLER(struct event);
void DISK2_HANDLER(struct event);
void avgSize();
void avgAtComp();
void avgRespTime();
void total();
#define SIZE  200

// initialization of variables used throughout program
int front = -1;
int rear = -1;
int cpuF = -1;
int cpuR = -1;
int disk1F = -1;
int disk1R = -1;
int disk2F = -1;
int disk2R = -1;
int disk1Size = 0;
int disk2Size = 0;
int clock;
int currentDisk1 = 0;
int currentDisk2 = 0;
int currentCPU = 0;
int totalSizePQ = 0;
int totalSizeCPU = 0;
int totalSizeDISK1 = 0;
int totalSizeDISK2 = 0;
int avgSizePQ = 0;
int avgSizeCPU = 0;
int avgSizeDISK1 = 0;
int avgSizeDISK2 = 0;
int totalUseCPU = 0;
int totalUseDISK1 = 0;
int totalUseDISK2 = 0;
int avgUseCPU = 0;
int avgUseDISK1 = 0;
int avgUseDISK2 = 0;

// array of structs create for each component
struct event pQ[SIZE];
struct event cpuQ[SIZE];
struct event disk1Q[SIZE];
struct event disk2Q[SIZE];

// struct event object created
struct event job;
struct event jobF;
struct event cJob;


int INIT_TIME, FIN_TIME, ARRIVE_MIN, ARRIVE_MAX, CPU_MIN, CPU_MAX, DISK1_MIN, DISK1_MAX, DISK2_MIN, DISK2_MAX, SEED;
double QUIT_PROB;

// file pointers created
FILE *ptr, *ptr2, *ptr3;


// main method
int main(void){
srand(time(NULL));

// ptr opens values.txt to read variables
ptr = fopen("values.txt", "r");


// these variables are read from values.txt file and assigned values to the correct variables
fscanf(ptr, "%d %d %d %d %d %lf %d %d %d %d %d %d", &SEED, &INIT_TIME, &FIN_TIME, &ARRIVE_MIN, &ARRIVE_MAX, &QUIT_PROB, &CPU_MIN, &CPU_MAX, &DISK1_MIN, &DISK1_MAX, &DISK2_MIN, &DISK2_MAX);

// close the values.txt file
fclose(ptr);

QUIT_PROB = QUIT_PROB * 10;


puts("when      what");

// ptr opens another file called output.txt that prints all the outputs of the program
ptr2 = fopen("output.txt", "w");
fprintf(ptr2, "when	what\n");


bool result=false;

setup();

// main loop goes on until priority queue is empty or when the time exceeds FIN_TIME
do{
    //checks if queue is full
    if(rear == SIZE -1){
	fprintf(ptr2, "%d	simulation finished", FIN_TIME);
        printf("%d      simulation finished", FIN_TIME);
	break;

    }

    result = isEmpty(); // checks if queue is empty
    bubbleSort(pQ); // sort all jobs
    cJob = delete(); // pops of current job in pQ
 
    // these conditions check the state of the job that has been popped of from pQ and print the correct statement depending on the eventState
    if(cJob.eventState == 1){
	JOB_ARRIVAL_EVENT(cJob);
	fprintf(ptr2, "%d	job %d arrives\n", cJob.execTime, cJob.id);
	printf("%d	job %d arrives\n",cJob.execTime, cJob.id);
    }else if(cJob.eventState == 2){
	fprintf(ptr2, "%d	job %d finished at CPU\n", cJob.execTime, cJob.id);
	printf("%d	job %d finished at CPU\n",cJob.execTime, cJob.id);
	currentCPU = 0;
    }else if(cJob.eventState == 3){
	fprintf(ptr2, "%d	job %d finished at DISK 1\n", cJob.execTime, cJob.id );
	printf("%d	job %d finished at DISK 1\n",cJob.execTime, cJob.id);
	currentDisk1 = 0;
    }else if(cJob.eventState == 4){
	fprintf(ptr2, "%d	job %d finished at DISK 2\n", cJob.execTime, cJob.id);
        printf("%d      job %d finished at DISK 2\n",cJob.execTime, cJob.id);
        currentDisk2 = 0;
	
    }else if(cJob.eventState == 5){
	fprintf(ptr2, "%d	job %d exits the system\n", cJob.execTime, cJob.id);
	printf("%d	job %d exits the system\n", cJob.execTime, cJob.id);
	currentCPU = 0;
    }else if(cJob.eventState == 6){
        break;
        fprintf(ptr2, "%d       simulation finished", cJob.execTime);
        printf("%d      simulation finished", cJob.execTime);
    }


}while( ((result != true) && (clock<100000))   );

// close accss to outputs.txt file
fclose(ptr2);

// ptr3 opens a new file called avg.txt to write the calculations of avg
ptr3 = fopen("avg.txt", "w");

//calls these methods to print the appropriate calculations
avgSize();
avgAtComp();

// close ptr3
fclose(ptr3);

}

// function that adds job to pQ by updating variable rear to the next index of array 
void add(struct event job, struct event q[]){
    totalSizePQ++;
    if(rear == SIZE -1){
	printf("Queue is full");
	
    }
    if(front == -1){
	front = 0;
    }

    rear+=1;
    q[rear] = job;
}

// delete job from pQ by assigning the current job to steuct event variable pop.
struct event delete(){
    struct event pop;
    int temp;
    pop.id = pQ[front].id;
    pop.execTime = pQ[front].execTime;
    pop.eventState = pQ[front].eventState;

    int i;
    for(i = front; i < SIZE; i++){
	if(   pop.id == pQ[i].id   &&  pQ[i].eventState < pop.eventState    ){
	    temp = pop.eventState;
	    pop.eventState = pQ[i].eventState;
	    pQ[i].eventState = temp;	
	}
    }



    front = front+1; // increment front by 1 so index is pointed to the next job in the queue

    return pop;
}

// bubble sort method that sort each job with the next job in pQ
void bubbleSort(struct event pQueue[]){
int i,j;
struct event temp;
for(i = front; i<SIZE;i++){
    for(j=i+1;j<SIZE-1;j++){
        if( (pQueue[i].execTime > pQueue[j].execTime) && (pQueue[j].execTime !=0)){
	    temp.id = pQueue[i].id;
	    temp.execTime = pQueue[i].execTime;
	    temp.eventState = pQueue[i].eventState;

	    pQueue[i].id =pQueue[j].id;
	    pQueue[i].execTime =pQueue[j].execTime;
	    pQueue[i].eventState =pQueue[j].eventState;

	    pQueue[j].id = temp.id;
	    pQueue[j].execTime = temp.execTime;
            pQueue[j].eventState = temp.eventState;
	}
    }
}
}

// function to check ipQ is empty
bool isEmpty(){
    bool result = false;
    if(front == SIZE-1){
        result = true;     
    }
    return result;
}

// function that adds job to cpuQ by incrementing cpuR by 1. cpuR points to the next index where the next job will be added
void addToCPU(struct event job, struct event q[]){
    totalSizeCPU++;
    
    if(cpuR == SIZE -1){
        printf("Queue is full");
    }

    if(cpuF == -1){
        cpuF = 0;
    }

    cpuR+=1;
    q[cpuR] = job;
}

// function deletes current job from cpuQ by increment cpuF by 1
struct event deleteFromCPU(){
    struct event pop;
    pop.id = cpuQ[cpuF].id;
    pop.execTime = cpuQ[cpuF].execTime;
    pop.eventState = cpuQ[cpuF].eventState;

    cpuF = cpuF+1;

    return pop;
}

// function adds job to disk1Q by increment disk1R by 1
void addToDisk1(struct event job, struct event q[]){
    totalSizeDISK1++;
    if(disk1R == SIZE -1){
        printf("Queue is full");
    }

    if(disk1F == -1){
        disk1F = 0;
    }

    disk1R+=1;
    q[disk1R] = job;
}

// function deletes job from disk1Q by incrementing disk1F
struct event deleteFromDisk1(){
    struct event pop;
    pop.id = disk1Q[disk1F].id;
    pop.execTime = disk1Q[disk1F].execTime;
    pop.eventState = disk1Q[disk1F].eventState;
    disk1F = disk1F+1;

    return pop;
}

// function addsjob to disk2Q by incrementing disk2R by 1
void addToDisk2(struct event job, struct event q[]){
    totalSizeDISK2++;
    if(disk2R == SIZE -1){
        printf("Queue is full");
    }
    if(disk2F == -1){
        disk2F = 0;
    }
    disk2R+=1;
    q[disk2R] = job;
}

// function adds to disk2 by incrementing disk2F by 1
struct event deleteFromDisk2(){
    struct event pop;
    pop.id = disk2Q[disk2F].id;
    pop.execTime = disk2Q[disk2F].execTime;
    pop.eventState = disk2Q[disk2F].eventState;

    disk2F = disk2F+1;

    return pop;
}

// function for getting size of queue
int size(struct event q[],int f,int r){
    int amt = 0;

    amt = r - f;

    return amt;

}

// function to initialize the first and last job and add it to pQ
void setup(){
    job.id = 1;
    job.execTime = INIT_TIME;
    job.eventState = 1;
    add(job,pQ);
    
    jobF.execTime = FIN_TIME;
    jobF.eventState = 6;
    add(jobF,pQ);    
}

// function creates a new job and adds it to pQ
void JOB_ARRIVAL_EVENT(struct event currentJob){
    struct event nJob;
    int r;
    clock = currentJob.execTime;
    r = rand()%ARRIVE_MAX + ARRIVE_MIN;
    
    nJob.id = currentJob.id + 1;
    nJob.execTime = r + clock;
    nJob.eventState = 1;
    add(nJob,pQ);

    // adds the job pooped of from pQ into cpuQ
    addToCPU(currentJob,cpuQ);
	// calls CPU_HANDLER()
    CPU_HANDLER(currentJob);
}

//function to handle the jobs. 
void CPU_HANDLER(struct event currentJob){
    struct event cpu_fin_event;
    struct event cpuJob;
    int r1,r2,r3,s1,s2,num;

	// checks if there is a job being serviced in CPU
    if(currentCPU == 0){
	r3 = rand()%10 + 1;
	r1 = rand()%CPU_MAX + CPU_MIN;
	// deletes job fromcpuQ
	cpuJob = deleteFromCPU();
	currentCPU =cpuJob.execTime;
	cpuJob.execTime = r1 + clock;
        totalUseCPU+=r1;
	// updates eventState
	if(r3 == QUIT_PROB){
	    cpuJob.eventState = 5;
	}else{
	    cpuJob.eventState = 2;
	}

	// add update job to pQ
        add(cpuJob,pQ);

	// gets zise of both disk1Q and disk2Q
	s1 = size(disk1Q,disk1F,disk1R);
	s2 = size(disk2Q,disk2F,disk2R);
	//checks which one is smaller and adds job to appropriate queue
	if(s1 < s2){
	    addToDisk1(cpuJob,disk1Q);
	    DISK1_HANDLER(cpuJob);

	}else if(s2 < s1){
            addToDisk2(currentJob,disk2Q);
	    DISK2_HANDLER(cpuJob);
	}else if(s1 == s2){
	    num = rand()%10+1;
	    if(num == 3){
                addToDisk1(cpuJob,disk1Q);
		DISK1_HANDLER(cpuJob);
	    }else{
	        addToDisk2(cpuJob,disk2Q);
        	DISK2_HANDLER(cpuJob);
	    }
	}
	
	// if CPU not busy do the same thing
    }else{
        r2 = rand()%CPU_MAX + CPU_MIN;
	cpu_fin_event = deleteFromCPU();
        cpu_fin_event.execTime = r2 + clock;
        cpu_fin_event.eventState = 2;
    
        add(cpu_fin_event,pQ);
	s1 = size(disk1Q,disk1F,disk1R);
        s2 = size(disk2Q,disk2F,disk2R);

        if(s1 < s2){
               addToDisk1(cpu_fin_event,disk1Q);
	       DISK1_HANDLER(cpu_fin_event);
        }else if(s2 < s1){
               addToDisk2(cpu_fin_event,disk2Q);
	       DISK2_HANDLER(cpu_fin_event);
        }else if(s1 == s2){
            num = rand()%10+1;
            if(num == 3){
                addToDisk1(cpu_fin_event,disk1Q);
		DISK1_HANDLER(cpu_fin_event);
            }else{
                addToDisk2(cpu_fin_event,disk2Q);
		DISK2_HANDLER(cpu_fin_event);
            }
        }
    }
}


// function thathandles jobs in disk1Q
void DISK1_HANDLER(struct event currentJob){
struct event dJob;
struct event disk1_fin_event;
    int r1,r2;
    struct event temp;
	// checks if disk1 is busy servicing a job and updates eventState
    if(currentDisk1 == 0){
        r1 = rand()%DISK1_MAX + DISK1_MIN;
	totalUseDISK1+=r1;
        dJob = deleteFromDisk1();
        currentDisk1 = 1;
        dJob.execTime = r1 + clock;
        dJob.eventState = 3;
        add(dJob,pQ);
	// if not busy does the same thing
    }else{
        r2 = rand()%DISK1_MAX + DISK1_MIN;
        disk1_fin_event.id = currentJob.id;
        disk1_fin_event.execTime = r2 + clock;
        disk1_fin_event.eventState = 3;

	// added to pQ
        add(disk1_fin_event,pQ);
    }
}

void DISK2_HANDLER(struct event currentJob){
struct event dJob;
struct event disk2_fin_event;
    int r1,r2;
    
    if(currentDisk2 == 0){
        r1 = rand()%DISK2_MAX + DISK2_MIN;
	totalUseDISK2+=r1;
        dJob = deleteFromDisk2();
        currentDisk2 = 1;
        dJob.execTime = r1 + clock;
        dJob.eventState = 4;
        add(dJob,pQ);
    }else{
        r2 = rand()%DISK2_MAX + DISK2_MIN;
        disk2_fin_event.id = currentJob.id;
        disk2_fin_event.execTime = r2 + clock;
        disk2_fin_event.eventState = 4;
    
        add(disk2_fin_event,pQ);
    }
}

// gets avg size of each queue
void avgSize(){
    int s1,s2,s3,s4;
    s1 = size(disk1Q,disk1F,disk1R);
    s2 = size(disk2Q,disk2F,disk2R);    
    s3 = size(cpuQ,cpuF,cpuR);
    s4 = size(pQ,front,rear);

    avgSizeCPU = totalSizeCPU/s3;
    avgSizeDISK1 = totalSizeDISK1/s1;
    avgSizeDISK2 = totalSizeDISK2/s2;
    avgSizePQ = totalSizePQ/s4;

    fprintf(ptr3, "AVERAGE SIZE AT EACH COMPONENT:\n\n");
    fprintf(ptr3, "Avg CPU Size: %d\n", avgSizeCPU);
    fprintf(ptr3, "Avg DISK1 Size: %d\n", avgSizeDISK1);
    fprintf(ptr3, "Avg DISK2 Size: %d\n", avgSizeDISK2);
    fprintf(ptr3, "Avg PQ Size: %d\n\n", avgSizePQ);
}

// gets avg use of each component
void avgAtComp(){
    avgUseCPU = totalUseCPU/FIN_TIME;    
    avgUseDISK1 = totalUseDISK1/FIN_TIME;
    avgUseDISK2 = totalUseDISK2/FIN_TIME;

    fprintf(ptr3, "AVERAGE UTILIZATION AT EACH COMPONENT:\n\n");
    fprintf(ptr3, "Avg CPU Use: %d\n", avgUseCPU);
    fprintf(ptr3, "Avg DISK1 Use: %d\n", avgUseDISK1);
    fprintf(ptr3, "Avg DISK2 Use: %d\n", avgUseDISK2);



}
