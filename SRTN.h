void SRTN(PQueue *priorityQueue){
    // mainQueue = create_queue();
    signal(SIGUSR2 , finish_handler);
    int lastPid = -1;
    while (flag || !p_queue_isEmpty(priorityQueue) || currentlyRunningProcess != NULL)
    {
        if(p_queue_isEmpty(priorityQueue)){
            continue;
        }
        Process *readyProcess = p_queue_peek(priorityQueue);
        if(currentlyRunningProcess == NULL){
            pop_p_queue(priorityQueue);
            process_exit_queue(readyProcess);
            process_run(readyProcess);
        }else if(readyProcess ->id != lastPid &&            compare_remaining_time( readyProcess , currentlyRunningProcess)){
            lastPid = readyProcess->id;
            process_enter_queue(currentlyRunningProcess);
            print_current_state(currentlyRunningProcess , 1);
            insert_time_order(priorityQueue , currentlyRunningProcess);
            process_end(currentlyRunningProcess);
            pop_p_queue(priorityQueue);
            process_exit_queue(readyProcess);
            process_run(readyProcess);
        }
    }
    // while (!queue_isEmpty(mainQueue))
    // {
    //     Process *finishedP = queue_peek(mainQueue);
    //     deQueue(mainQueue);
    //     free(finishedP);
    // }
    
    
}
