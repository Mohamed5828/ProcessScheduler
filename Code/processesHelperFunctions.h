#include <assert.h>
#include "limits.h"
#include "headers.h"
typedef struct msgbuff
{
    long mtype;
    Process process;
} msgbuff;

int totalRunningTime = 0;
int totalWaitingTime = 0;

bool processRunningFlag;
PQueue *mainPiorityQueue;
Queue *mainQueue;
PQueue *prePQueue;
Process *currentlyRunningProcess;
int flag = 1;

float STD = 0;
float totalWTA= 0.0;
float meanWTA= 0.0;
float totalWTASq= 0.0;


void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}
void up(int sem){
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;
    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}

int process_remaining_time(const Process* const process);

void process_error_null(const Process *p){
    if (p == NULL){
    printf("\nError in printing process info, the process is NULL");
    exit(0);
}
}

void print_full_info(const Process* const process){
if (process == NULL){
    printf("\nError in printing process info, the process is NULL");
    exit(0);
}
    printf("\n*********************************************\n");
    printf("id                : %d\n", process->id);
    printf("pid               : %d\n", process->pid);
    printf("arrivalTime       : %d\n", process->arrivalTime);
    printf("runtime           : %d\n", process->runningTime);
    printf("priority          : %d\n", process->priority);
    printf("enterQueue        : %d\n", process->enterTime);
    printf("quitQueue         : %d\n", process->quitTime);
    printf("executedTime      : %d\n", process->executionTime);
    printf("remaining time    : %d\n", process_remaining_time(process));
    printf("waitingTime       : %d\n", process->waitingTime);
    printf("running           : %d\n", process->isRunning);
    printf("startedBefore     : %d\n", process->isCreated);
    printf("Memory            : %d\n", process->memeorySize);
    printf("Memory Address    : %p\n", process->memoryPTR);
    printf("\n*********************************************\n");
    fflush(stdout);
}

//still needs to be edited
void print_current_state(const Process* const process , int state){
    FILE *outputFile = fopen("scheduler.log" , "a+");
    if(outputFile == NULL){
        perror("unable to open output file");
        exit(-1);
    };
    int currentTime = getClk();
    fprintf(outputFile, "%s", "At time ");
    fprintf(outputFile, "%d ", currentTime);
    fprintf(outputFile, "%s", "process ");
    fprintf(outputFile, "%d ", process->id);
    switch (state)
    {
    case 0:
        fprintf(outputFile , "%s" , "Started");
        break;
    case 1:
        fprintf(outputFile , "%s" , "Stopped");
        break;
    case 2:
        fprintf(outputFile , "%s" , "Resumed");
        break;
    case 3:
        fprintf(outputFile , "%s" , "Finished");
        break;
    
    default:
        perror("Wronge state code");
        exit(-1);
    }

fprintf(outputFile, "%s", "arr ");
    fprintf(outputFile, "%d ", process->arrivalTime);
    fprintf(outputFile, "%s", "total ");
    fprintf(outputFile, "%d ", process->runningTime);
    fprintf(outputFile, "%s", "remain ");
    fprintf(outputFile, "%d ", process_remaining_time(process));
    fprintf(outputFile, "%s", "wait ");
    fprintf(outputFile, "%d ", process->waitingTime);
    //save this value for scheduler.perf file
    totalWaitingTime += process->waitingTime;

    if (state == 3) {
        int TA = currentTime - process->arrivalTime;
        float WTA;
        if (process->runningTime == 0)
            WTA = 0;
        else WTA = (float) TA / process->runningTime;
        //save this values for scheduler.perf file
        totalWTA += WTA;
        totalWTASq += WTA * WTA ;

        fprintf(outputFile, "%s", "TA ");
        fprintf(outputFile, "%d ", TA);
        fprintf(outputFile, "%s", "WTA ");
        fprintf(outputFile, "%.2f", WTA);
    }


    fprintf(outputFile , "%s" , "\n");
    fclose(outputFile);
}

void process_run(Process* process){
    //buddy_malloc in process run 

    process_error_null(process);
    currentlyRunningProcess = process;
    if(process->isCreated == 1){
        print_current_state(process , 2);
    }else{
        print_current_state(process , 0);
    }
    currentlyRunningProcess ->waitingTime = getClk() - currentlyRunningProcess->arrivalTime;
    //then we create the process
    int pid = fork();
    
    if (pid == -1){
        perror("Error in forking");
        exit(-1);
    }
    if(pid == 0){

       char remaining_time_str[20];
        sprintf(remaining_time_str, "%d", process_remaining_time(process));
        if (process->pid != -1){
            printf("a process with id = %d is created with pid = %d current time %d\n" ,process ->id , getpid() , getClk());
            // Without the call to fflush(stdout), there is a possibility that some of the output from the first process may not be visible to the user before the new process starts
            fflush(stdout);
        }
        char *args[] = {"./process.out" , remaining_time_str, NULL};
        int execResult = execvp(args[0] , args);
             if (execResult == -1) {
            perror("Error in execl");
            exit(-1);
        }
    }
    //log pid here 
    process->pid = pid;
    print_current_state(process , 0);
    printf("\n=Running:");
    printf("\n Current time = %d", getClk());
    print_full_info(process);
       
}

void process_end(const Process* const process){
    process_error_null(process);
 printf("\n=Stopping:");
    printf("\n Current time = %d\n", getClk());
    printf("Signal Stop has been sent to the process %d\n", process->pid);
    int kill_process = kill(process ->pid , SIGKILL);
    if(kill_process == -1){
        printf("Error in sending signal stop\n");
        exit(-1);
    }
    currentlyRunningProcess = NULL;
}


int process_remaining_time(const Process* const process){
    process_error_null(process);
    int remaining_time = process->runningTime - process->executionTime;
    assert(remaining_time >= 0);
    return remaining_time;

}

void process_enter_queue( Process* const process){
    process_error_null(process);
    int current_time = getClk();
    process->isRunning = 0;
    process->isCreated = 1;
    process->enterTime = current_time;
    process->executionTime += current_time - process->quitTime;
    assert(process->executionTime >= 0);
    // printf("\n=Entered Queue:");
    // printf("\n Current time = %d", current_time);
    // print_full_info(process);
}

void process_exit_queue( Process* const process){
    process_error_null(process);
    int current_time = getClk();
    process->isRunning = 1;
    process->quitTime = current_time;
    process->waitingTime += current_time - process->enterTime;
    if (process->pid != -1){
    // printf("current time: %d waiting time: %d entering queue time: %d" ,current_time , process->waitingTime , process->enterTime);
    assert(process->waitingTime >= 0);
    // printf("\n=Left Queue:");
    // printf("\n Current time = %d", current_time);
    // print_full_info(process);
    }
}

void finish_handler(int signum){
    printf("process %d has finished execution\n" , currentlyRunningProcess->pid);
    printf("Current clock : %d \n", getClk());
    process_enter_queue(currentlyRunningProcess);
    print_current_state(currentlyRunningProcess , 3);
    // enQueue(mainQueue , currentlyRunningProcess);
    buddy_free(currentlyRunningProcess->memoryPTR);
    currentlyRunningProcess = NULL;
    signal(SIGUSR2 , finish_handler);
}

int compare_remaining_time(const Process* const onTop , const Process* const runningProcess){
    process_error_null(runningProcess);
    process_error_null(onTop);
    int runningExecution = getClk() - runningProcess->quitTime;
    int topReminder = onTop->runningTime - onTop->executionTime;
    int currentReminder = runningProcess->runningTime - runningProcess->executionTime - runningExecution;

    return (currentReminder < 0) ? 0 : (topReminder < currentReminder);
}


