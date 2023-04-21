#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scheduling_algorithms.c"


#define NUM_ROUNDS 5
#define MAX_PROCESSES 100
#define MAX_QUANTA 100.0f
#define MAX_SRT 4
#define MAX_EXPECTED_BURST_TIME 10
#define MIN_EXPECTED_BURST_TIME 1

typedef struct {
    float avg_turnaround_time;
    float avg_waiting_time;
    float avg_response_time;
    float avg_throughput;
} scheduling_stats;

enum {
    FCFS,
    SJF,
    SRT,
    ROUND_ROBIN,
    HPF_P,
    HPF_NP,
    NUM_ALGORITHMS
};

typedef struct {
    int code;
    char name[50];
    void (*run_algo)(struct process *, int);
} algorithm;

algorithm algorithms[NUM_ALGORITHMS] = {
    {FCFS, "First Come First Serve", .run_algo = run_fcfs},
    {SJF, "Shortest Job First", .run_algo = run_sjf},
    {SRT, "Shortest Remaining Time First", .run_algo = run_srt},
    {ROUND_ROBIN, "Round Robin", .run_algo = run_rr},
    {HPF_P, "Highest Priority First Preemptive", .run_algo = run_hpf_p},
    {HPF_NP, "Highest Priority First Non-Preemptive", .run_algo = run_hpf_np}
};

typedef void (*scheduler_fn)(struct process *, int);


typedef struct {
    scheduling_stats stats[NUM_ALGORITHMS][NUM_ROUNDS];
} scheduling_stats_table;


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
        processes[i].priority = rand() % MAX_SRT + 1;  // Generate a random SRT between 1 and MAX_SRT
    }
}

void reset_processes(struct process processes[], int n) {
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time  = processes[i].expected_burst_time ; 
        processes[i].completion_time = 0 ;
        processes[i].waiting_time    = 0 ;
        processes[i].turnaround_time = 0 ;
        processes[i].response_time   = 0 ;
        processes[i].start_time      = -1 ;
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
    printf("-------------------------------------------------\n");
    printf("Process ID | Arrival Time | Run Time | Priority |\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%10d | %12d | %8d | %8d |\n", processes[i].id, processes[i].arrival_time, processes[i].expected_burst_time, processes[i].priority);
    }
    printf("--------------------------------------------------\n");
    printf("Total No. of Processes : %d\n", n);
    printf("--------------------------------------------------\n");
}
void calculate_statistics(struct process *processes, int n, scheduling_stats_table **table, int algorithm, int round)
{
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    float total_cpu_time = 0;
    float avg_turnaround_time = 0.0;
    float avg_waiting_time = 0.0;
    float avg_response_time = 0.0;
    float avg_throughput = 0.0;
    
    for (int i = 0; i < n; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
        total_response_time += processes[i].response_time;
        total_cpu_time += processes[i].expected_burst_time;
    }

    avg_turnaround_time = total_turnaround_time / n;
    avg_waiting_time = total_waiting_time / n;
    avg_response_time = total_response_time / n;
    avg_throughput = (float)n / total_cpu_time;

    (*table)->stats[algorithm][round].avg_turnaround_time = avg_turnaround_time;
    (*table)->stats[algorithm][round].avg_waiting_time = avg_waiting_time;
    (*table)->stats[algorithm][round].avg_response_time = avg_response_time;
    (*table)->stats[algorithm][round].avg_throughput = avg_throughput;

    // Print the calculated statistics
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Throughput: %.2f\n", avg_throughput);
}

void run_algorithm(const char* name, struct process processes[], int n, scheduler_fn run_scheduler, scheduling_stats_table* stats_table, int algorithm, int round) {
    printf("\n*********************************************************************************\n");
    printf("\t\t\t\t%s\n", name);
    printf("*********************************************************************************\n");

    // Execute the scheduling algorithm and modify the completion_time,
    // turnaround_time, and start_time of each process.
    // Calculate the round statistics based on the modified process data.
    
    run_scheduler(processes, n);    

    calculate_statistics(processes, n, &stats_table, algorithm, round);
}

void print_stat_table(scheduling_stats_table* stats_table) {
    // Print header
    printf("%-40s%-20s%-20s%-20s%-20s\n", "Algorithm", "Avg. Turnaround Time", "Avg. Waiting Time", "Avg. Response Time", "Avg. Throughput");
    float total_algo_turnaround_time  = 0.0;
    float total_algo_waiting_time     = 0.0;
    float total_algo_response_time    = 0.0;
    float total_algo_avg_throughput   = 0.0;
    // Loop through algorithms
    for (int i = 0; i < NUM_ALGORITHMS; i++) {
        // Initialize variables for row data and totals
        float total_turnaround_time = 0.0;
        float total_waiting_time = 0.0;
        float total_response_time = 0.0;
        float total_throughput = 0.0;

        // Loop through rounds
        for (int j = 0; j < NUM_ROUNDS; j++) {
            // Get scheduling stats for algorithm and round
            scheduling_stats stats = stats_table->stats[i][j];

            // Add to totals
            total_turnaround_time += stats.avg_turnaround_time;
            total_waiting_time += stats.avg_waiting_time;
            total_response_time += stats.avg_response_time;
            total_throughput += stats.avg_throughput;
        }

        // Calculate averages
        float avg_turnaround_time = total_turnaround_time / NUM_ROUNDS;
        float avg_waiting_time = total_waiting_time / NUM_ROUNDS;
        float avg_response_time = total_response_time / NUM_ROUNDS;
        float avg_throughput = total_throughput / NUM_ROUNDS;

        // Print row
        printf("%-40s%-20.2f%-20.2f%-20.2f%-20.2f\n", algorithms[i].name, avg_turnaround_time, avg_waiting_time, avg_response_time, avg_throughput);

        // Add to total algorithm totals
        total_algo_turnaround_time += avg_turnaround_time;
        total_algo_waiting_time += avg_waiting_time;
        total_algo_response_time += avg_response_time;
        total_algo_avg_throughput += avg_throughput;
    }

    // Print total row
    printf("%-40s%-20.2f%-20.2f%-20.2f%-20.2f\n", "Total", total_algo_turnaround_time, total_algo_waiting_time, total_algo_response_time, total_algo_avg_throughput);
}


int main() {
    scheduling_stats_table stats_table;
    int n;

    printf("Enter the number of processes: \n");
    scanf("%d", &n);
    struct process processes[n];

    // Generate and run processes for each algorithm
    for (int i = 0; i < NUM_ROUNDS; i++) {
        printf("\n*********************************************************************************\n");
        printf("\t\t\t\tRound %d\n", i + 1);
        printf("*********************************************************************************\n");

        generate_processes(processes, n); // Generate n random processes
        sort_processes_on_arrival(processes, n); // Sort the processes by arrival time
        print_process_table(processes, n); // Print process table
        for (int j = 0; j < NUM_ALGORITHMS; j++) {
            reset_processes(processes, n);
            algorithm algo = algorithms[j];
            run_algorithm(algo.name, processes, n, algo.run_algo, &stats_table, algo.code, i);
        }
    }

    printf("\n\n\n*********************************************************************************\n");
    printf("\t\t\t\tStatistics\n");
    printf("*********************************************************************************\n");
    print_stat_table(&stats_table);
    return 0;
}
