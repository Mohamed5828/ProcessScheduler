void RR ( Queue *queue , int quantum){

    int lastPid = -1;
    int prevProcessTime = getClk(), currentProcessTime , prevProcessRunTime; 
    signal(SIGUSR2 , finish_handler);
    while (flag || !queue_isEmpty(queue) || currentlyRunningProcess != NULL){
    if(queue_isEmpty(queue)){
        continue;
    }
    Process *onTop = queue_peek(queue);
    currentProcessTime = getClk();
    prevProcessRunTime = currentProcessTime - prevProcessTime;
    if (currentlyRunningProcess == NULL){
        deQueue(queue);
        process_exit_queue(onTop);
        process_run(onTop); // currentlyrunningprocess = onTop;
        prevProcessTime = currentProcessTime;

    }else if(prevProcessRunTime >= quantum && prevProcessRunTime % quantum == 0 && onTop->id != lastPid){
        lastPid = onTop->id;
        prevProcessTime = currentProcessTime;
        int reminder = process_remaining_time(onTop) - (getClk() - onTop->quitTime);

        assert(reminder >= 0);
        if(remainder > 0 ){
            process_enter_queue(currentlyRunningProcess);
            print_current_state(currentlyRunningProcess , 1);
            enQueue(queue , currentlyRunningProcess);
            process_end(currentlyRunningProcess);
            deQueue(queue);
            process_exit_queue(onTop);
            process_run(onTop);
        }
    }
   }
    printf("\n\n**************RR Terminated at  time = %d*************\n\n", getClk());
}

//    while (!queue_isEmpty(queue))
//    {
//     currentlyRunningProcess = queue_peek(queue);
//     process_run(currentlyRunningProcess);
//     currentlyRunningProcess -> executionTime+= quantum;
//     deQueue(queue);
//     if (currentlyRunningProcess->executionTime < currentlyRunningProcess->runningTime){
//         enQueue(queue , currentlyRunningProcess)
//     }
//    }