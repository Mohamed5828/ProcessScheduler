#include "processesHelperFunctions.h"

int totalPRunningTime;
int numberOfProcesses;
int msgQId;
int pSchedGeneratorSem;
int quantum = -1;
msgbuff message;

void clearResources(int);

void read_file(Queue *processQueue){
    FILE *file = fopen("processes.txt" , "r");
    int id, runningTime, arrivalTime, priority, memorySize;
    totalPRunningTime = 0;
    numberOfProcesses = 0;
    char first_line [30];
    fgets(first_line , 30 ,file);
    while(fscanf(file , "%d\t%d\t%d\t%d\t%d" , &id , &arrivalTime , &runningTime , &priority ,&memorySize) != EOF){
        Process *process = (Process*) malloc(sizeof(Process));
        process->id = id;
        process->arrivalTime = arrivalTime;
        process->priority = priority;
        process->runningTime = runningTime;
        process->isRunning=false;
        process->isCreated = false;
        process->quitTime = 0;
        process->pid = -1;
        process->executionTime = 0;
        process->waitingTime = 0;
        process->memeorySize = memorySize;
        process->memoryPTR = NULL;

        numberOfProcesses++;
        totalPRunningTime += runningTime ;
        enQueue(processQueue , process);
    }
    fclose(file);
}

int schedulerAlgorithm(){
    int algorithm;
    printf("Which scheduling algorithm do you want?\n");
    printf("=======================================\n");
    printf("[1] Non-preemptive Highest Priority First (HPF)\n");
    printf("[2] Shortest Remaining time Next (SRTN)\n");
    printf("[3] Round Robin (RR)\n");
    printf("=======================================\n");
    scanf("%d", &algorithm);
    if (algorithm > 3 || algorithm < 1) {
        printf("Enter a number from 1 to 3\n");
        scanf("%d", &algorithm);
    }
    if(algorithm == 3 ){
        printf("enter the Quantum -must be a whole number-\n");
        scanf("%d" , &quantum);
        if (quantum < 0)
        {
            printf("Enter a number bigger than 0\n");
            scanf("%d", &quantum);
        }
    }
    return algorithm;
}

void intialize_semaphore(){
    pSchedGeneratorSem = semget(SEMKEY, 1, 0666 | IPC_CREAT);
    if(pSchedGeneratorSem == -1){
        perror("Error in creating Semaphore");
        exit(-1);
    }
    union Semun semun;
    semun.val = 0;
    if (semctl(pSchedGeneratorSem , 0, SETVAL , semun) == -1){
        perror("Error in internalizing Semaphore");
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

void send_process(Process *process){
    message.mtype = 7;
    message.process = *process;
    int send_val = msgsnd(msgQId, &message , sizeof(message.process) , !IPC_NOWAIT);
    if (send_val == -1){
        perror("Error in sending message");
    }
}

void stop_process(){
    Process *process = (Process*) malloc(sizeof(Process));
    process->id = -1;
    // printf("Sending process with id = -1\n");
    send_process(process);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, clearResources);
    // TODO Initialization
    intialize_semaphore();
    intialize_messageQueue();
    // 1. Read the input files.
    Queue *processQueue = create_queue();
    read_file(processQueue);
    // 2. Read the chosen scheduling algorithm and its parameters, if there are any from the argument list.
    int algroithm = schedulerAlgorithm();
    // 3. Initiate and create the scheduler and clock processes.
    int clkpid = fork();
    if (clkpid == 0){
        char *args[] = {"./clk.out" , NULL};
        execv(args[0] , args);
    }

    int schedulerpid = fork();
    if (schedulerpid == -1)
        perror("error in forking");
    if (schedulerpid == 0)
    {
        printf("sched son fork");
        //run the scheduler file
        int length = snprintf(NULL ,0 , "%d" , algroithm);
        char *algo = malloc(length +1 );
        snprintf(algo , length + 1 , "%d" , algroithm);

        length = snprintf(NULL, 0, "%d", numberOfProcesses);
        char *procNum = malloc(length + 1);
        snprintf(procNum, length + 1, "%d", numberOfProcesses);

        length = snprintf(NULL, 0, "%d", totalPRunningTime);
        char *totalRT = malloc(length + 1);
        snprintf(totalRT, length + 1, "%d", totalPRunningTime);

        length = snprintf(NULL, 0, "%d", quantum);
        char *quant = malloc(length + 1);
        snprintf(quant, length + 1, "%d", quantum);

        char *args[] = {"./scheduler.out", algo, procNum, totalRT, quant, NULL};
        execv(args[0], args);
    }
    
    // 4. Use this function after creating the clock process to initialize clock.
    initClk();
    int clk;
    // To get time use this function. 
    // TODO Generation Main Loop
    // 5. Create a data structure for processes and provide it with its parameters.
    // 6. Send the information to the scheduler at the appropriate time.
    while (!queue_isEmpty(processQueue))
    {
        Process *process = queue_peek(processQueue);
        clk = getClk();
        while (process->arrivalTime != clk)
        {
            clk = getClk();
        }
        printf("Process with id = %d arrived at %d\n" ,process->id , clk );

        //put the process in the message queue
        send_process(process);
        //send a signal to the schedular to be ready for the coming process
        kill(schedulerpid , SIGUSR1);
        // Down the semaphore to make sure that the scheduler has pushed the process to the queue
        down(pSchedGeneratorSem);
        
        //remove it from the queue
        deQueue(processQueue);

    }
    //send a process with id = -1
    //to inform the schedular that there is no other processes coming
    stop_process();
    kill(schedulerpid, SIGUSR1);
    //wait for the schedular to finish before clearing the clock resources
    waitpid(schedulerpid, NULL, 0);
    printf("==================Process generator terminated======================\n");
    
    // 7. Clear clock resources
    destroyClk(true);
}

void clearResources(int signum)
{
    //TODO Clears all resources in case of interruption

    msgctl((msgQId) , IPC_RMID , (struct msqid_ds *) 0);
    semctl(pSchedGeneratorSem , 0 ,IPC_RMID , (struct msqid_ds *) 0);
    // semctl(pSchedGeneratorSem , 0 , IPC_RMID , (union Semun) 0);
    printf("resource cleared");
}
