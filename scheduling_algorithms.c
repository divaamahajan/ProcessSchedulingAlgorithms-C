#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "queues.c"


/* Comparison function for qsort, compares arrival times of two processes. */
int compare_expected_burst_time (const void *p1, const void *p2) {
    return ((struct process *) p1)->expected_burst_time  - ((struct process *) p2)->expected_burst_time ;
}

struct process*  run_fcfs(struct process processes[], int n) {
    // Run the First-Come First-Served scheduling algorithm
    // Code for FCFS
}

struct process* run_sjf(struct process processes[], int n) {
    // Run the Shortest Job First scheduling algorithm
    // Code for SJF
    struct queue* ready_queue = create_queue(n);
    int current_time = 0;
    int num_completed_processes = 0;
    int next_process_idx = 0;    
    struct process* completed_processes = malloc(n * sizeof(struct process));    
    
    // Check if there are any processes to schedule
    if (n == 0) {
        printf("No processes to schedule.\n");
        return completed_processes;
    }
    
    // Check if the first process has arrived
    if (processes[0].arrival_time > current_time){
        printf("\nCPU idle from time %d to time %d\n", current_time, processes[0].arrival_time );
        current_time = processes[0].arrival_time;
        printf("\nCurrent Time : %d\n", current_time );
    }

    // Loop until all processes are completed
    while (num_completed_processes < n) {            
        // Add any processes that have arrived to the ready queue
        for (int i = next_process_idx; i < n; i++) { 
            struct process p = processes[i];
            if (p.expected_burst_time < 1){continue;}
            if (p.arrival_time > current_time){
                next_process_idx = i; 
                break;
            }
            if (!p.completion_time) {
                // Insert the process in the ready queue in its correct position based on expected burst time  
                if (p.expected_burst_time < 1){continue;}          
                enqueue_sorted_burst_time(ready_queue, p, compare_expected_burst_time);
                next_process_idx = i+1;

            }
        }

        // If the ready queue is empty, add the next process to the queue
        if (is_empty(ready_queue) && num_completed_processes < n) {
            printf("\nCPU idle from time %d to time %d\n", current_time, processes[next_process_idx].arrival_time );
            enqueue_sorted_burst_time(ready_queue, processes[next_process_idx],compare_expected_burst_time);
            current_time = processes[next_process_idx].arrival_time;
            printf("\nCurrent Time : %d\n", current_time );
            next_process_idx++ ;
        }

        printf("\nReady Queue ==> ");
        print_queue(ready_queue);


        // Get the process with the shortest expected burst time from the ready queue
        struct process current_process = dequeue(ready_queue);

        if (current_process.id == completed_processes[-1].id) {continue;}

        // Update the process's start time and completion time
        current_process.start_time = current_time;
        current_process.completion_time = current_time + current_process.expected_burst_time;

        printf("Executing Process P%d from time %d to time %d \n", (int)current_process.id, (int)current_process.start_time , (int)current_process.completion_time);
    
        // Update waiting time, response time, and completion time
        current_process.waiting_time = current_time - current_process.arrival_time;
        current_process.response_time = current_process.start_time - current_process.arrival_time;
        current_process.completion_time = current_time + current_process.expected_burst_time;
        current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;

        printf("| %7s | %7s | %5s | %8s | %8s | %8s | %10s |\n", "Arrival", "Wait", "Start", "Response", "RunTime", "Complete", "Turnaround");
        printf("| %7d | %7d | %5d | %8d | %8d | %8d | %10d |\n",
        current_process.arrival_time,
        current_process.waiting_time,
        current_process.start_time,
        current_process.response_time,
        current_process.expected_burst_time,
        current_process.completion_time,
        current_process.turnaround_time);

        
        // Add current process to array of running processes
        completed_processes[num_completed_processes] = current_process;
        num_completed_processes++;

        // num_completed_processes++;
        
        current_time = current_process.completion_time;
        
        printf("\nCurrent Time : %d\n", current_time );
        
    }    
    // Free memory allocated for the queue
    free_queue(ready_queue);
    return completed_processes;
}


struct process*  run_srt(struct process processes[], int n) {
    // Run the Shortest Remaining Time scheduling algorithm
    // Code for SRT
}

struct process*  run_rr(struct process processes[], int n) {
    // Run the Round Robin scheduling algorithm
    // Code for RR
}

struct process*  run_hpf_np(struct process processes[], int n) {
    // Run the Non-preemptive Highest Priority First scheduling algorithm
    // Code for HPF Non-preemptive
}

struct process*  run_hpf_p(struct process processes[], int n) {
    // Run the Preemptive Highest Priority First scheduling algorithm
    // Code for HPF Preemptive
}
