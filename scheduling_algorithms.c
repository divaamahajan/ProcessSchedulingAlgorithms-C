#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "queues.c"
#include "utils.h"
#include "srt.c"

void print_execution(int pid, int start_time, int end_time)
{
    char str[12]; // increase buffer size to 12
    if (pid < 1)
    {
        return;
        snprintf(str, sizeof(str), "-");
    }
    else
    {
        snprintf(str, sizeof(str), "P%d", pid);
    }
    for (int j = start_time; j < end_time; j++)
    {
        printf("%s ", str);
    }
}

// Function to calculate waiting time, turnaround time, response time, and completion time
void calculate_times(struct process *p, int current_time)
{
    p->start_time = current_time;
    p->completion_time = current_time + p->expected_burst_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->expected_burst_time;
    p->response_time = p->start_time - p->arrival_time;
}

// Function to calculate HPF waiting time, turnaround time, response time, and completion time
void calculate_times_hpf(struct process *p, int current_time)
{
    p->completion_time = current_time + p->expected_burst_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->expected_burst_time;
    p->response_time = p->start_time - p->arrival_time;
}

/* Comparison function for enqueue_sorted_burst_time, compares expected burst times of two processes. */
int compare_expected_burst_time(struct process *processes, int idx1, struct process *p2, int idx2)
{
    return processes[idx1].expected_burst_time - p2->expected_burst_time;
}

// Comparison function for sorting processes by priority

void run_fcfs(struct process *processes, int n)
{
    // Run the First-Come First-Served scheduling algorithm
    // Code for FCFS
    
    // Create a queue to hold the ready processes
    struct queue* ready_queue = create_queue();

    // Initialize the current time and the index of the next process to execute
     int current_time = 0;
    int num_completed_processes = 0;
    int next_process_idx = 0;    

    // Check if there are any processes to schedule
    if (n == 0) {
        printf("No processes to schedule.\n");
        return;
    }
    
    // Check if the first process has arrived
    if (processes[0].arrival_time > current_time){
        print_execution(0, current_time, processes[0].arrival_time );
        current_time = processes[0].arrival_time;
    }
    
    // Loop until all processes have been executed
    while (next_process_idx < n) {
        // Add any arrived processes to the ready queue
        for (int i = next_process_idx; i < n; i++) {
            struct process p = processes[i];
            if (p.arrival_time <= current_time && p.expected_burst_time > 0 && !p.completion_time) {
                enqueue(ready_queue, i);
                next_process_idx = i + 1;
            } else {
                break;
            }
        }

    // Check if there are any processes in the ready queue
        if (!is_empty(ready_queue)) {
            // Get the next process from the ready queue
            int current_idx = dequeue(ready_queue);
            struct process *current_process_ptr = &processes[current_idx];
            
            // Update the process's start time, completion time, and other times
            current_process_ptr->start_time = current_time;
            current_process_ptr->completion_time = current_time + current_process_ptr->expected_burst_time;
            calculate_times(current_process_ptr, current_time);

    // Print the process execution
            print_execution((int)current_process_ptr->id, (int)current_process_ptr->start_time, (int)current_process_ptr->completion_time);
            
            // Update the current time
            current_time = current_process_ptr->completion_time;
        } else {
            // If the ready queue is empty, move the current time to the arrival time of the next process
            print_execution(0, current_time, processes[next_process_idx].arrival_time);
            current_time = processes[next_process_idx].arrival_time;
        }
    }
    
    // Free memory allocated for the queue
    free_queue(ready_queue);
}

void run_sjf(struct process *processes, int n)
{
    // Run the Shortest Job First scheduling algorithm
    // Code for SJF
    struct queue *ready_queue = create_queue();
    int current_time = 0;
    int num_completed_processes = 0;
    int next_process_idx = 0;

    // Check if there are any processes to schedule
    if (n == 0)
    {
        printf("No processes to schedule.\n");
        return;
    }

    // Check if the first process has arrived
    if (processes[0].arrival_time > current_time)
    {
        print_execution(0, current_time, processes[0].arrival_time);
        current_time = processes[0].arrival_time;
    }

    // Loop until all processes are completed
    while (num_completed_processes < n && current_time < 100)
    {
        // Add any processes that have arrived to the ready queue
        for (int i = next_process_idx; i < n; i++)
        {
            struct process p = processes[i];
            if (p.expected_burst_time < 1)
            {
                continue;
            }
            if (p.arrival_time > current_time)
            {
                next_process_idx = i;
                break;
            }
            if (!p.completion_time)
            {
                // Insert the process in the ready queue in its correct position based on expected burst time
                if (p.expected_burst_time < 1)
                {
                    continue;
                }
                enqueue_sorted_burst_time(ready_queue, processes, i, compare_expected_burst_time);
                next_process_idx = i + 1;
            }
        }

        // If the ready queue is empty, add the next process to the queue
        if (is_empty(ready_queue) && num_completed_processes < n)
        {
            print_execution(0, current_time, processes[next_process_idx].arrival_time);
            enqueue_sorted_burst_time(ready_queue, processes, next_process_idx, compare_expected_burst_time);
            current_time = processes[next_process_idx].arrival_time;
            next_process_idx++;
        }
        // Get the process with the shortest expected burst time from the ready queue
        // struct process* current_process_ptr = dequeue(ready_queue);
        int current_idx = dequeue(ready_queue);

        // Update the process's start time and completion time
        processes[current_idx].start_time = current_time;
        processes[current_idx].completion_time = current_time + processes[current_idx].expected_burst_time;
        print_execution((int)processes[current_idx].id, (int)processes[current_idx].start_time, (int)processes[current_idx].completion_time);

        // Update waiting time, response time, and completion time
        processes[current_idx].waiting_time = current_time - processes[current_idx].arrival_time;
        processes[current_idx].response_time = processes[current_idx].start_time - processes[current_idx].arrival_time;
        processes[current_idx].turnaround_time = processes[current_idx].completion_time - processes[current_idx].arrival_time;

        // Add current process to array of running processes
        num_completed_processes++;

        current_time = processes[current_idx].completion_time;
    }
    // Free memory allocated for the queue
    free_queue(ready_queue);
}

void run_srt(struct process *processes, int n)
{
    // Run the Shortest Remaining Time scheduling algorithm
    // Code for SRT
    findavgTime(processes, n);
}

void run_rr(struct process *processes, int n)
{
    // Run the Round Robin scheduling algorithm
    // Code for RR
    // Create a queue for ready processes
    struct queue *ready_queue = create_queue();

    // Define time quantum for Round Robin
    int time_quantum = 1;

    // Initialize variables
    int current_time = 0;
    int next_process_idx = 0;
    int num_completed_processes = 0;

    // Check if there are any processes to schedule
    if (n == 0)
    {
        printf("No processes to schedule.\n");
        return;
    }

    // Check if the first process has arrived
    if (processes[0].arrival_time > current_time)
    {
        print_execution(0, current_time, processes[0].arrival_time);
        current_time = processes[0].arrival_time;
    }

    // Loop until all processes are completed
    while (num_completed_processes < n)
    {
        // Add any processes that have arrived to the ready queue
        for (int i = next_process_idx; i < n; i++)
        {
            struct process p = processes[i];
            if (p.arrival_time > current_time)
            {
                next_process_idx = i;
                break;
            }
            if (!p.completion_time)
            {
                enqueue(ready_queue, i);
                next_process_idx = i + 1;
            }
        }

        // If the ready queue is empty, add the next process to the queue
        if (is_empty(ready_queue) && num_completed_processes < n)
        {
            print_execution(0, current_time, processes[next_process_idx].arrival_time);
            current_time = processes[next_process_idx].arrival_time;
            enqueue(ready_queue, next_process_idx);
            next_process_idx++;
        }

        // Get the process at the front of the ready queue
        int current_idx = dequeue(ready_queue);
        struct process *current_process = &processes[current_idx];

        // Update start time if it's the first time this process has been scheduled
        if (current_process->start_time == -1)
        {
            current_process->start_time = current_time;
        }

        // Determine how much time the process will run for
        int remaining_time = current_process->expected_burst_time - current_process->elapsed_time;
        int run_time = (remaining_time > time_quantum) ? time_quantum : remaining_time;

        // Update elapsed time for the current process
        current_process->elapsed_time += run_time;

        // Update current time
        current_time += run_time;

        // Print execution of process
        print_execution(current_process->id, current_time - run_time, current_time);

        // Check if process is completed
        if (current_process->elapsed_time == current_process->expected_burst_time)
        {
            num_completed_processes++;
            calculate_times(current_process, current_time);
        }
        else
        {
            enqueue(ready_queue, current_idx);
        }
    }

    // Free memory allocated for the queue
    free_queue(ready_queue);
}

// Function to run the Non-preemptive Highest Priority First scheduling algorithm
void run_hpf_np(struct process *processes, int n)
{
    // Run the Non-preemptive Highest Priority First scheduling algorithm
    // Code for HPF Non-preemptive
    // Creation of a queue of process indices
    struct queue *process_queue_1 = create_queue();
    struct queue *process_queue_2 = create_queue();
    struct queue *process_queue_3 = create_queue();
    struct queue *process_queue_4 = create_queue();

    struct hpf_queue *final_queue_1 = create_hpf_queue();
    struct hpf_queue *final_queue_2 = create_hpf_queue();
    struct hpf_queue *final_queue_3 = create_hpf_queue();
    struct hpf_queue *final_queue_4 = create_hpf_queue();

    int current_time = 0;
    int num_completed_processes = 0;
    int next_process_idx = 0;

    // Check if there are any processes to schedule
    if (n == 0)
    {
        printf("No processes to schedule.\n");
        return;
    }

    // Loop until all processes are completed
    while (num_completed_processes < n && current_time < 100)
    {
        // Check for new arrivals
        while (next_process_idx < n && processes[next_process_idx].arrival_time <= current_time)
        {
            // Add the process index to the corresponding queue based on its priority
            int process_idx = next_process_idx;
            struct process *p = &processes[process_idx];
            switch (p->priority)
            {
            case 1:
                enqueue(process_queue_1, process_idx);
                break;
            case 2:
                enqueue(process_queue_2, process_idx);
                break;
            case 3:
                enqueue(process_queue_3, process_idx);
                break;
            case 4:
                enqueue(process_queue_4, process_idx);
                break;
            default:
                printf("Invalid priority for process %d.\n", p->id);
            }
            next_process_idx++;
        }

        // Select the process with the highest priority
        struct process *p = NULL;
        if (!is_empty(process_queue_1))
        {
            int process_idx = dequeue(process_queue_1);
            p = &processes[process_idx];
        }
        else if (!is_empty(process_queue_2))
        {
            int process_idx = dequeue(process_queue_2);
            p = &processes[process_idx];
        }
        else if (!is_empty(process_queue_3))
        {
            int process_idx = dequeue(process_queue_3);
            p = &processes[process_idx];
        }
        else if (!is_empty(process_queue_4))
        {
            int process_idx = dequeue(process_queue_4);
            p = &processes[process_idx];
        }

        // If a process was selected, run it
        if (p != NULL)
        {
            // Set the start time of the process
            p->start_time = current_time;

            // Calculate the waiting time, turnaround time, response time, and completion time of the process
            calculate_times(p, current_time);
            switch (p->priority)
            {
            case 1:
                enqueue_hpf(final_queue_1, p);
                break;
            case 2:
                enqueue_hpf(final_queue_2, p);
                break;
            case 3:
                enqueue_hpf(final_queue_3, p);
                break;
            case 4:
                enqueue_hpf(final_queue_4, p);
                break;
            }
            // Update the current time
            current_time += p->expected_burst_time;

            print_execution(p->id, p->start_time, current_time);

            // Increment the number of completed processes
            num_completed_processes++;
        }
        else
        {
            print_execution(0, current_time, current_time + 1);
            // If no process was selected, increment the current time
            current_time++;
        }
    }
    calculate_statistics_pq(final_queue_1, 1);
    calculate_statistics_pq(final_queue_2, 2);
    calculate_statistics_pq(final_queue_3, 3);
    calculate_statistics_pq(final_queue_4, 4);
    // Free the memory used by the process queues
    free_queue(process_queue_1);
    free_queue(process_queue_2);
    free_queue(process_queue_3);
    free_queue(process_queue_4);
    free_hpf_queue(final_queue_1);
    free_hpf_queue(final_queue_2);
    free_hpf_queue(final_queue_3);
    free_hpf_queue(final_queue_4);
}

void run_hpf_p(struct process *processes, int n)
{
    // run the high prioirty first scheduler with preemption
    //  create 4 priority queues

    // create 4 final_queues
    struct hpf_queue *final_queue_1 = create_hpf_queue();
    struct hpf_queue *final_queue_2 = create_hpf_queue();
    struct hpf_queue *final_queue_3 = create_hpf_queue();
    struct hpf_queue *final_queue_4 = create_hpf_queue();

    struct hpf_queue *priority_queue_1 = create_hpf_queue();
    struct hpf_queue *priority_queue_2 = create_hpf_queue();
    struct hpf_queue *priority_queue_3 = create_hpf_queue();
    struct hpf_queue *priority_queue_4 = create_hpf_queue();

    int current_time = 0;
    int num_completed_processes = 0;
    int process_idx = 0;
    int new_process_idx = 0;
    struct process *scheduled_process = NULL;
    struct process *proc = &processes[process_idx];

    while (num_completed_processes != n || scheduled_process != NULL)
    {
        // Check for new arrivals
        if (scheduled_process != NULL)
        {
            // if a process is currently running, add it to priority queue
            if (scheduled_process->priority == 1)
                enqueue_hpf(priority_queue_1, scheduled_process);
            if (scheduled_process->priority == 2)
                enqueue_hpf(priority_queue_2, scheduled_process);
            if (scheduled_process->priority == 3)
                enqueue_hpf(priority_queue_3, scheduled_process);
            if (scheduled_process->priority == 4)
                enqueue_hpf(priority_queue_4, scheduled_process);
            scheduled_process = NULL;
        }

        if (num_completed_processes != n)
        {
            while (process_idx < n && proc->arrival_time <= current_time)
            {
                // Add the process index to the corresponding queue based on its priority
                switch (proc->priority)
                {
                case 1:
                    enqueue_hpf(priority_queue_1, proc);
                    break;
                case 2:
                    enqueue_hpf(priority_queue_2, proc);
                    break;
                case 3:
                    enqueue_hpf(priority_queue_3, proc);
                    break;
                case 4:
                    enqueue_hpf(priority_queue_4, proc);
                    break;
                }
                if (priority_queue_1)
                {
                    sort_priority(priority_queue_1, compare_priority);
                }
                if (priority_queue_2)
                {
                    sort_priority(priority_queue_2, compare_priority);
                }
                if (priority_queue_3)
                {
                    sort_priority(priority_queue_3, compare_priority);
                }
                if (priority_queue_4)
                {
                    sort_priority(priority_queue_4, compare_priority);
                }
                process_idx++;
                proc = &processes[process_idx];
            }
        }
        if (scheduled_process == NULL)
        {
            if (!is_hpf_queue_empty(priority_queue_1))
            {
                scheduled_process = dequeue_hpf(priority_queue_1);
            }
            else if (!is_hpf_queue_empty(priority_queue_2))
            {
                scheduled_process = dequeue_hpf(priority_queue_2);
            }
            else if (!is_hpf_queue_empty(priority_queue_3))
            {
                scheduled_process = dequeue_hpf(priority_queue_3);
            }
            else if (!is_hpf_queue_empty(priority_queue_4))
            {
                scheduled_process = dequeue_hpf(priority_queue_4);
            }
            if (current_time >= 100 && scheduled_process->start_time == -1)
            {
                scheduled_process = NULL;
                continue;
            }
        }
        if (scheduled_process != NULL)
        {
            print_exec(scheduled_process->id);
            if (scheduled_process->start_time == -1)
            {
                scheduled_process->start_time = current_time;
            }
            scheduled_process->remaining_time--;
            if (scheduled_process->remaining_time <= 0)
            {
                scheduled_process->completion_time = current_time;
                calculate_times_hpf(scheduled_process, current_time);
                switch (scheduled_process->priority)
                {
                case 1:
                    enqueue_hpf(final_queue_1, scheduled_process);
                    break;
                case 2:
                    enqueue_hpf(final_queue_2, scheduled_process);
                    break;
                case 3:
                    enqueue_hpf(final_queue_3, scheduled_process);
                    break;
                case 4:
                    enqueue_hpf(final_queue_4, scheduled_process);
                    break;
                }
                scheduled_process = NULL;
                num_completed_processes++;
            }
        }
        else
        {
            print_exec(-1);
        }
        current_time++;
    }
    calculate_statistics_pq(final_queue_1, 1);
    calculate_statistics_pq(final_queue_2, 2);
    calculate_statistics_pq(final_queue_3, 3);
    calculate_statistics_pq(final_queue_4, 4);
    // Free the memory used by the process queues
    free_hpf_queue(final_queue_1);
    free_hpf_queue(final_queue_2);
    free_hpf_queue(final_queue_3);
    free_hpf_queue(final_queue_4);
    free_hpf_queue(priority_queue_1);
    free_hpf_queue(priority_queue_2);
    free_hpf_queue(priority_queue_3);
    free_hpf_queue(priority_queue_4);
}
