#include "processesHelperFunctions.h"
#include "HPF.h"
#include "SRTN.h"
#include "RR.h"

int numberOfProcesses = 0;
int msgQId;
int pSchedGeneratorSem;
int algroithm;
int quantum = -1;

msgbuff message;


Process* create_process(int id, int arrivalTime, int runningTime, int priority, int enterTime, int quitTime, int executionTime, int waitingTime, boolean isRunning, boolean isCreated, int pid ,int memorySize);

void intialize_semaphore();

void intialize_messageQueue();

void get_process(int);

void print_queue(int);

void create_scheduler_log();

void create_scheduler_perf();

int main(int argc, char *argv[])
{
    create_scheduler_log();
    signal(SIGUSR1 , get_process);
    signal(SIGTRAP , print_queue);
    initClk();
    intialize_semaphore();
    intialize_messageQueue();
    initialize_tree();
    
    algroithm = atoi(argv[1]);
    numberOfProcesses = atoi(argv[2]);
    totalRunningTime = atoi(argv[3]);
    quantum = atoi(argv[4]);

    switch (algroithm)
    {
    case 1:
        mainPiorityQueue = create_priority_queue();
        HPF(mainPiorityQueue);
        break;
    
    case 2:
        mainPiorityQueue = create_priority_queue();
        SRTN(mainPiorityQueue);
        break;
    
    case 3:
        mainQueue = create_queue();
        RR(mainQueue , quantum);
        break;
    }
    
    printf("\n\n=================Scheduler Terminated at %d=================",getClk());
    create_scheduler_perf();
    destroyClk(true);
    return 0;
}

void intialize_semaphore(){
    pSchedGeneratorSem = semget(SEMKEY, 1, 0666 | IPC_CREAT);
    if(pSchedGeneratorSem == -1){
        perror("Error in creating Semaphore");
        exit(-1);
    }
}

void intialize_messageQueue(){
    msgQId = msgget(PROSCH , 0666 | IPC_CREAT);
    if (msgQId == -1){
        perror("Error in create message queue");
        exit(-1);
    }
}

Process* create_process(int id, int arrivalTime, int runningTime, int priority, int enterTime, int quitTime, int executionTime, int waitingTime, boolean isRunning, boolean isCreated, int pid , int memorySize){
    Process *process = (Process*) malloc(sizeof(Process));
    process->arrivalTime = arrivalTime;
    process->id = id;
    process->isCreated = isCreated;
    process->runningTime = runningTime;
    process->priority = priority;
    process->enterTime = enterTime;
    process->quitTime = quitTime;
    process->executionTime = executionTime;
    process->waitingTime = waitingTime;
    process->isRunning = isRunning;
    process->remainingTime = process_remaining_time(process);
    process->pid = pid;
    process->memeorySize = memorySize;
    process->memoryPTR = NULL;
        return process;
}

void add_HPF_queue(Process process){
    if (process.id != -1){
        Process *newProcess = create_process(
             process.id, process.arrivalTime, process.runningTime, process.priority, process.enterTime,  process.quitTime, process.executionTime, process.waitingTime,process.isRunning ,process.isCreated, process.pid,process.memeorySize
        );
        insert_ordered(mainPiorityQueue , newProcess);
    }
    
}
void add_RR_queue(Process process){
    if (process.id != -1){
        Process *newProcess = create_process(
             process.id, process.arrivalTime, process.runningTime, process.priority, process.enterTime,  process.quitTime, process.executionTime, process.waitingTime,process.isRunning ,process.isCreated, process.pid,process.memeorySize
        );
        enQueue(mainQueue , newProcess);
    }
    
}
void add_SRTN_queue(Process process){
    if (process.id != -1){
        Process *newProcess = create_process(
             process.id, process.arrivalTime, process.runningTime, process.priority, process.enterTime,  process.quitTime, process.executionTime, process.waitingTime,process.isRunning ,process.isCreated, process.pid,process.memeorySize
        );
        insert_time_order(mainPiorityQueue , newProcess);
    }
    
}

void get_process(int signum){
    int rec_val = msgrcv(msgQId, &message , sizeof(message.process), 7 , !IPC_NOWAIT);
    printf("message received with id: %d\n" , message.process.id);
    fflush(stdout);
    if(rec_val == -1){
        perror("Error in receive");
    }
    switch (algroithm)
    {
    case 1:
        add_HPF_queue(message.process);
        break;
    case 2:
        add_SRTN_queue(message.process);
        break;
    case 3:
        add_RR_queue(message.process);
        break;
    }
    up(pSchedGeneratorSem);
    if(message.process.id == -1){
        flag = 0;
    }
    printf("getProcess terminated");
}

void print_queue(int signum){
    printf("i have recieved a signal %d\n", signum);
    PQueueNode *start = mainPiorityQueue->head;
    while (start != NULL)
    {
        print_full_info(start->data);
        start = start->next;
    }
    
}
void create_scheduler_log(){
    FILE *outputFile;
    outputFile = fopen("scheduler.log" , "w");
    fprintf(outputFile , "%s" , "#At time x process y state arr w total z remain y wait k\n");
    fclose(outputFile);
}

void create_scheduler_perf(){
    FILE *outputFile;
    outputFile = fopen("scheduler.perf", "w");

    fprintf(outputFile , "%s" , "CPU utilization = ");

    fprintf(outputFile, "%d%%\n", (totalRunningTime * 100 / getClk()));

    //Avg WTA
    fprintf(outputFile, "%s", "Avg WTA = ");
    fprintf(outputFile, "%.2f\n", (totalWTA / numberOfProcesses));

    //Avg Waiting
    fprintf(outputFile, "%s", "Avg Waiting = ");
    fprintf(outputFile, "%.2f\n", ((float) totalWaitingTime / numberOfProcesses));

    //Std WTA
    meanWTA = totalWTA / numberOfProcesses;
    float x = ((totalWTA) - (numberOfProcesses * meanWTA * meanWTA ));
    // STD = sqrt(abs(x) / numberOfProcesses);

    fprintf(outputFile, "%s", "Std WTA = ");
    // fprintf(outputFile, "%.2f\n", STD);

    fclose(outputFile);
}

