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
    buddy_free(currentlyRunningProcess->memoryPTR);
    free(currentlyRunningProcess);
    signal(SIGUSR2 , HPF_finished);
}

void HPF(PQueue *pq){
    PQueue *processQueue;
    Process *onTopProcess;
    Process *readyProcess;
    signal(SIGUSR2 , HPF_finished);
    processRunningFlag = false;
    printf("HPF Starting \n");
    processQueue = pq;
while (flag || !p_queue_isEmpty(processQueue) || processRunningFlag){
    
    //this following three line is to always at the start peek and pop the preQueue and if there still no memeory for the peeked process it will be added to preQueue again later
  if(!p_queue_isEmpty(prePQueue)){
        // printf("preQueue is not empty\n");
        Process *processNeedMemory = p_queue_peek(prePQueue);
        // printf("current process in preQueue have an id of: %d\n",processNeedMemory->id);

        pop_p_queue(prePQueue);
        insert_ordered(mainPiorityQueue ,processNeedMemory);
    }
    if (p_queue_isEmpty(processQueue)){
        continue;
    }if(!processRunningFlag){
    readyProcess = p_queue_peek(processQueue);
        process_run(readyProcess);
    // if(readyProcess->memoryPTR == NULL){
    //     insert_ordered(prePQueue , readyProcess);
    // }else{
        pop_p_queue(processQueue);    
    // }
    }
    
}
}