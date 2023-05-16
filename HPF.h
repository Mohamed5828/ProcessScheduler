//what we doing 
//first we get the queue and the process
//we call the function insert_ordered
//the process should then be added in order in the queue
//every 1 second the queue is checking if a new process comes with higher priority 
//or is it when a new processes comes we check so it comes down to how we get the processes
//after the process time required is given the queue should pop the process


void HPF_finished(int signum){
    processRunningFlag = false;
    Process *current_process;
    // process_run(current_process);
    currentlyRunningProcess ->executionTime = currentlyRunningProcess ->runningTime;
    print_current_state(currentlyRunningProcess , 3);
    free(currentlyRunningProcess);
    signal(SIGUSR2 , HPF_finished);
}

void HPF(PQueue *pq){
    PQueue *processQueue;
    Process *readyProcess;
    signal(SIGUSR2 , HPF_finished);
    processRunningFlag = false;
    printf("HPF Starting \n");
    processQueue = pq;
while (flag || !p_queue_isEmpty(processQueue) || processRunningFlag){
    if (p_queue_isEmpty(processQueue)){
        continue;
    }if(!processRunningFlag){
    readyProcess = p_queue_peek(processQueue);
    pop_p_queue(processQueue);    
    process_run(readyProcess);
    }
    
}
}