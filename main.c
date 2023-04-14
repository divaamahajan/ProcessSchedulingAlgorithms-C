#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduling_algorithms.c"

#define MAX_PROCESSES 100
#define MAX_QUANTA 100.0f
#define MAX_PRIORITY 4
#define MAX_EXPECTED_BURST_TIME 10
#define MIN_EXPECTED_BURST_TIME 1


void generate_processes(struct process processes[], int n) {
// Code for generating n random processes
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        // Generate a random arrival time between 0 and MAX_QUANTA 
        processes[i].arrival_time = ((double)rand() / RAND_MAX) * MAX_QUANTA;  
        // Generate a random expected runtime between MIN_EXPECTED_BURST_TIME and MAX_EXPECTED_BURST_TIME
        processes[i].expected_burst_time = ((double)rand() / RAND_MAX) * (MAX_EXPECTED_BURST_TIME - MIN_EXPECTED_BURST_TIME) + MIN_EXPECTED_BURST_TIME;
        processes[i].priority = rand() % MAX_PRIORITY + 1;  // Generate a random priority between 1 and MAX_PRIORITY
        processes[i].remaining_time  = 0 ; 
        processes[i].completion_time = 0 ; 
        processes[i].waiting_time    = 0 ; 
        processes[i].turnaround_time = 0 ; 
        processes[i].response_time   = 0 ; 
        processes[i].start_time      = 0 ; 
    }
}

/* Comparison function for qsort, compares arrival times of two processes. */
int compare_arrival_time(const void *p1, const void *p2) {
    return ((struct process *) p1)->arrival_time - ((struct process *) p2)->arrival_time;
}

void sort_processes_on_arrival(struct process processes[], int n) {
// Code for sorting the processes by arrival time
    //Sorts an array of processes in ascending order of their arrival times.
    // Use quicksort algorithm for better performance.
    qsort(processes, n, sizeof(struct process), compare_arrival_time);
}

void print_process_table(struct process processes[], int n) {
// Code for printing the process table
    printf("\n\n");
    printf("ID | Arrival Time | Expected Burst Time | Priority\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("P%2d | %12.0d | %19.2d | %8d\n", processes[i].id, processes[i].arrival_time, processes[i].expected_burst_time, processes[i].priority);
    }
}


void calculate_statistics(struct process processes[], int n, float* avg_turnaround_time, float* avg_waiting_time, float* avg_response_time) {
// Code for calculating the statistics
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;

    // Calculate the total turnaround, waiting, and response times
    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
        total_response_time += processes[i].response_time;
    }

    // Calculate the average turnaround, waiting, and response times
    *avg_turnaround_time = total_turnaround_time / n;
    *avg_waiting_time = total_waiting_time / n;
    *avg_response_time = total_response_time / n;

    // Print the calculated statistics
    printf("Average Waiting Time: %.2f\n", *avg_waiting_time);
    printf("Average Response Time: %.2f\n", *avg_response_time);
    printf("Average Turnaround Time: %.2f\n", *avg_turnaround_time);
}


float calculate_throughput(int n, int total_time) {
// Code for calculating the throughput
    return (float) n / total_time;
}

// Print a table of process information for each process in the array
void print_report_table(struct process processes[], int n) {
    printf("\n\n");
    printf("ID | Priority | Arrival | Waiting | Start | Response | Runtime | Completion | Turnaround\n");
    printf("-------------------------------------------------------------------------------------\n");
    // Loop through all processes in the array and print their information in a tabular format
    for (int i = 0; i < n; i++) {
        printf("P%2d | %8d | %7d | %7d | %5d | %8d | %7d | %10d | %10d\n", processes[i].id, processes[i].priority,
               processes[i].arrival_time, processes[i].waiting_time, processes[i].start_time, processes[i].response_time,
               processes[i].expected_burst_time, processes[i].completion_time, processes[i].turnaround_time);
    }
}


struct process* run_algorithm(struct process processes[], int n, int choice) {
    struct process* completed_processes;
    // Code for running the selected scheduling algorithm
    switch (choice) {
        case 1:
            completed_processes = run_fcfs(processes, n);
            printf("\nFirst-Come First-Served (FCFS) Scheduling Algorithm\n");
            break;
        case 2:
            completed_processes = run_sjf(processes, n);
            printf("\nShortest Job First (SJF) Scheduling Algorithm\n");
            break;
        case 3:
            completed_processes = run_srt(processes, n);
            printf("\nShortest Remaining Time (SRT) Scheduling Algorithm\n");
            break;
        case 4:
            completed_processes = run_rr(processes, n);
            printf("\nRound Robin (RR) Scheduling Algorithm\n");
            break;
        case 5:
            completed_processes = run_hpf_np(processes, n);
            printf("\nHighest Priority First (HPF) [Non-preemptive] Scheduling Algorithm\n");
            break;
        case 6:
            completed_processes = run_hpf_p(processes, n);
            printf("\nHighest Priority First (HPF) [Preemptive] Scheduling Algorithm\n");
            break;
        default:
            printf("Invalid choice\n");
            exit(0);
    }
    return completed_processes;
}


int main() {
    struct process processes[MAX_PROCESSES];
    int n, choice;
    float avg_turnaround_time, avg_waiting_time, avg_response_time, throughput;

    printf("Enter the number of processes: \n");
    scanf("%d", &n);

    generate_processes(processes, n); // Generate n random processes
    sort_processes_on_arrival(processes, n); // Sort the processes by arrival time
    print_process_table(processes, n); // Print process table

    printf("\n\nChoose the scheduling algorithm to run: \n");
    printf("1. First-Come First-Served (FCFS)\n");
    printf("2. Shortest Job First (SJF)\n");
    printf("3. Shortest Remaining Time (SRT)\n");
    printf("4. Round Robin (RR)\n");
    printf("5. Highest Priority First (HPF) [Non-preemptive]\n");
    printf("6. Highest Priority First (HPF) [Preemptive]\n");

    scanf("%d", &choice);

    struct process* completed_processes  = run_algorithm(processes, n, choice); // Run the selected scheduling algorithm
    calculate_statistics(completed_processes, n, &avg_turnaround_time, &avg_waiting_time, &avg_response_time); // Calculate statistics
    throughput = calculate_throughput(n, completed_processes[n - 1].completion_time - completed_processes[0].arrival_time); // Calculate throughput

    printf("\nCalculated statistics for the processes during the run:\n");
    printf("Average turnaround time: %.2f\n", avg_turnaround_time);
    printf("Average waiting time: %.2f\n", avg_waiting_time);
    printf("Average response time: %.2f\n", avg_response_time);

    printf("\nCalculated statistic for the algorithm for the run:\n");
    printf("Throughput: %.2f\n", throughput);

    print_report_table(completed_processes, n); // Print the process table
    return 0; // Exit the program
}
