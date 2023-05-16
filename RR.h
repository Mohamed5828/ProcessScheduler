void RR(Queue* queue, int quantum) {
    signal(SIGUSR2, finish_handler); // Register signal handler
    Process* running_process = NULL; // Currently running process
    int last_pid = -1; // ID of last process that ran
    int prev_time = getClk(); // Time of last process switch
    while (flag || !queue_isEmpty(queue) || running_process != NULL) {
        if (!queue_isEmpty(queue)) {
            Process* next_process = queue_peek(queue);
            int elapsed_time = getClk() - prev_time;
            if (running_process == NULL) {
                deQueue(queue);
                process_exit_queue(next_process);
                process_run(next_process);
                running_process = next_process;
                prev_time = getClk();
            } else if (elapsed_time >= quantum && next_process->id != last_pid) {
                last_pid = next_process->id;
                int remaining_time = process_remaining_time(running_process) - elapsed_time;
                if (remaining_time > 0) {
                    process_enter_queue(running_process);
                    print_current_state(running_process, 1);
                    enQueue(queue, running_process);
                    process_end(running_process)
                }
                deQueue(queue);
                process_exit_queue(next_process);
                process_run(next_process);
                running_process = next_process;
                prev_time = getClk();
            }
        }
    }
    printf("\n\n**************RR Terminated at time = %d*************\n\n", getClk());
}