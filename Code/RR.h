void RR(Queue* queue, int quantum) {
    signal(SIGUSR2, finish_handler); // Register signal handler
    int last_pid = -1; // ID of last process that ran
    int prev_time = getClk(); // Time of last process switch
    while (flag || queue_isEmpty(queue)==false || currentlyRunningProcess != NULL) {
        if (!queue_isEmpty(queue)) {
            Process* next_process = queue_peek(queue);
            int elapsed_time = getClk() - prev_time;
            if (currentlyRunningProcess == NULL) {
                deQueue(queue);
                //only 1 time when a process finished even before the process gets all the q
       
                process_exit_queue(next_process);
                process_run(next_process);
                prev_time = getClk();
            } else if (elapsed_time >= quantum && next_process->id != last_pid) {
                printQueue(queue);
                last_pid = next_process->id;
                int remaining_time = process_remaining_time(currentlyRunningProcess) - elapsed_time;
                if (remaining_time > 0) {
                    process_enter_queue(currentlyRunningProcess);
                    print_current_state(currentlyRunningProcess, 1);
                    enQueue(queue, currentlyRunningProcess);
                    process_end(currentlyRunningProcess);
                }
                deQueue(queue);
                process_exit_queue(next_process);
                process_run(next_process);
                prev_time = getClk();
            }
        }
    }
    printf("\n\n**************RR Terminated at time = %d*************\n\n", getClk());
}



// if(preQueue != NULL){
//     int queueSize = size(preQueue);
//     QueueNode *current = preQueue->front;
//     while (current != NULL)
//     {
//         void *memoryPTR = buddy_malloc(current->data->memeorySize);
//         if (memory_ptr) {
//             // printf("ALLOCATING\n");
//             processToBeAdded = queue_peek(preQueue);
//             deQueue(preQueue);
//             enQueue(queue , processToBeAdded);
//         }
//         current = current->next;    
//     }
    
// }

// void RR(Queue* queue, int quantum) {
//     signal(SIGUSR2, finish_handler); // Register signal handler
//     int last_pid = -1; // ID of last process that ran
//     int prev_time = getClk(); // Time of last process switch
//     while (flag || !queue_isEmpty(queue) || currentlyRunningProcess != NULL) {
//         if (!queue_isEmpty(queue)) {
//             Process* next_process = queue_peek(queue);
//             int elapsed_time = getClk() - prev_time;
//             if (currentlyRunningProcess == NULL) {
//                 deQueue(queue);
//                 process_exit_queue(next_process);
//                 process_run(next_process);
//                 prev_time = getClk();
//             } else if (elapsed_time >= quantum && (elapsed_time%quantum) == 0 && next_process->id != last_pid) {
//                 last_pid = next_process->id;
//                 prev_time = getClk();
//                 Process* premptedProcess = currentlyRunningProcess;

//                 int remaining_time = process_remaining_time(premptedProcess) - (getClk() - premptedProcess->quitTime);
//                 assert(remaining_time >= 0);
//                 if (remaining_time > 0) {
//                     process_end(currentlyRunningProcess);

//                     process_enter_queue(premptedProcess);
//                     print_current_state(premptedProcess, 1);
//                     enQueue(queue, premptedProcess);
//                 deQueue(queue);
//                 process_exit_queue(next_process);
//                 process_run(next_process);
//                 }
//             }
//         }
//     }
//     printf("\n\n**************RR Terminated at time = %d*************\n\n", getClk());
// }



