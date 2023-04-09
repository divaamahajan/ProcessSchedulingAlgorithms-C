#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>

#define MAX_PROCESSES 100
#define MAX_QUANTA 100.0f
#define MAX_PRIORITY 4
#define MAX_EXPECTED_BURST_TIME 10.0f
#define MIN_EXPECTED_BURST_TIME 0.1f

struct process {
    int id;
    int arrival_time;
    int expected_burst_time;
    int priority;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int response_time;
    int start_time;
};
// Define the struct for the queue
struct queue {
    struct process* array;
    int front;
    int rear;
    int capacity;
    int size;
};

// Function to create a new queue
struct queue* create_queue(int capacity) {
    struct queue* q = (struct queue*) malloc(sizeof(struct queue));
    q->array = (struct process*) malloc(capacity * sizeof(struct process));
    q->front = 0;
    q->rear = -1;
    q->capacity = capacity;
    q->size = 0;
    return q;
}

// Function to check if the queue is empty
int is_empty(struct queue* q) {
    return q->size == 0;
}

// Function to check if the queue is full
int is_full(struct queue* q) {
    return q->size == q->capacity;
}
void enqueue_sorted_burst_time(struct queue* q, struct process p, int (*compare)(const void*, const void*)) {
    if (is_full(q)) {
        printf("Error: Queue is full\n");
        return;
    }
    // If queue is empty, add the process to the front
    if (is_empty(q)) {
        q->array[q->front] = p;
        q->rear++;
        q->size++;
        return;
    }
    // Find the position to insert the new process
    int i;
    for (i = q->rear - 1; i >= q->front; i--) {
        if (compare(&p, &q->array[i]) >= 0) {
            break;
        }
        q->array[i + 1] = q->array[i];
    }
    // Insert the new process
    q->array[i + 1] = p;
    q->rear++;
    q->size++;
}


// Function to remove a process from the front of the queue
struct process dequeue(struct queue* q) {
    if (is_empty(q)) {
        printf("Error: Queue is empty\n");
        struct process p;
        p.id = -1;
        return p;
    }
    struct process p = q->array[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return p;
}

void print_queue(struct queue* q) {
    if (q->size == 0) {
        printf("Queue is empty\n");
        return;
    }
    for (int i = q->front; i <= q->rear; i++) {
        printf("%d", q->array[i].id);
        if (i != q->rear) {
            printf(" | ");
        }
    }
    printf("\n");
}

// Function to print the process table
void print_process_table(struct process processes[], int n) {
    printf("ID | Arrival Time | Expected Burst Time | Priority\n");
    printf("-------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%2d | %12.0d | %19.2d | %8d\n", processes[i].id, processes[i].arrival_time, processes[i].expected_burst_time, processes[i].priority);
    }
}

void generate_processes(struct process processes[], int n) {
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        // Generate a random arrival time between 0 and MAX_QUANTA 
        processes[i].arrival_time = ((double)rand() / RAND_MAX) * MAX_QUANTA;  
        // Generate a random expected runtime between MIN_EXPECTED_BURST_TIME and MAX_EXPECTED_BURST_TIME
        processes[i].expected_burst_time = ((double)rand() / RAND_MAX) * (MAX_EXPECTED_BURST_TIME - MIN_EXPECTED_BURST_TIME) + MIN_EXPECTED_BURST_TIME;
        processes[i].priority = rand() % MAX_PRIORITY + 1;  // Generate a random priority between 1 and MAX_PRIORITY
    }
}


/* Comparison function for qsort, compares arrival times of two processes. */
int compare_arrival_time(const void *p1, const void *p2) {
    return ((struct process *) p1)->arrival_time - ((struct process *) p2)->arrival_time;
}

/* Comparison function for qsort, compares arrival times of two processes. */
int compare_expected_burst_time (const void *p1, const void *p2) {
    return ((struct process *) p1)->expected_burst_time  - ((struct process *) p2)->expected_burst_time ;
}

/* Sorts an array of processes in ascending order of their arrival times. */
void sort_processes_on_arrival(struct process processes[], int n) {
    /* Use quicksort algorithm for better performance. */
    qsort(processes, n, sizeof(struct process), compare_arrival_time);
}

// Print a table of process information for each process in the array
void print_table(struct process processes[], int n) {
    printf("ID\tArrival\tExpected\tPriority\tCompletion\tWaiting\tTurnaround\tResponse\tStart\n");
    // Loop through all processes in the array and print their information in a tabular format
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t\t%d\t\t%d\t\t%d\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].arrival_time, processes[i].expected_burst_time,
               processes[i].priority, processes[i].completion_time, processes[i].waiting_time, processes[i].turnaround_time,
               processes[i].response_time, processes[i].start_time);
    }
}
void run_sjf(struct process processes[], int n) {
    struct queue* ready_queue = create_queue(n);
    float current_time = processes[0].arrival_time;
    int completed_processes = 0;
    int total_waiting_time = 0;
    int total_turnaround_time = 0;
    int total_response_time = 0;
    int next_process_idx = 0;
    
    
    // Loop until all processes are completed
    while (completed_processes < n) {             
        // Add any processes that have arrived to the ready queue
        for (int i = next_process_idx; i < n; i++) {
            if (processes[i].arrival_time > current_time){
                next_process_idx = i;
                break;
            }
            if (!processes[i].completion_time) {
                // Insert the process in the ready queue in its correct position based on expected burst time            
                enqueue_sorted_burst_time(ready_queue, processes[i], compare_expected_burst_time);
            }
        }

        printf("Ready Queue ||");
        print_queue(ready_queue);


        // Get the process with the shortest expected burst time from the ready queue
        struct process current_process = dequeue(ready_queue);

        // Update the process's start time and completion time
        current_process.start_time = current_time;
        current_process.completion_time = current_time + current_process.expected_burst_time;

        printf("Executing Process %d from time %d to time %d \n", (int)current_process.id, (int)current_process.start_time , (int)current_process.completion_time);
    
        // Update waiting time, response time, and completion time
        current_process.waiting_time = current_time - current_process.arrival_time;
        current_process.response_time = current_process.start_time - current_process.arrival_time;
        current_process.completion_time = current_time + current_process.expected_burst_time;
        current_process.turnaround_time = current_process.completion_time - current_process.arrival_time;
        
        // Update total waiting time, response time, and turnaround time
        total_waiting_time += current_process.waiting_time;
        total_response_time += current_process.response_time;
        total_turnaround_time += current_process.turnaround_time;
        
        // Print information about the completed process
        printf("Process %d completed.\n", current_process.id);
        printf("Waiting Time: %d, Response Time: %d, Turnaround Time: %d\n\n",
               current_process.waiting_time, current_process.response_time, current_process.turnaround_time);
        
        completed_processes++;

        
        current_time = current_process.completion_time;
        
        // If the ready queue is empty, add the next process to the queue
        if (is_empty(ready_queue) && completed_processes < n) {
            enqueue_sorted_burst_time(ready_queue, processes[next_process_idx],compare_expected_burst_time);
            current_time = processes[next_process_idx].arrival_time;
        }
        
    }
    
    // Calculate and print average waiting time, response time, and turnaround time
    float avg_waiting_time = (float) total_waiting_time / n;
    float avg_response_time = (float) total_response_time / n;
    float avg_turnaround_time = (float) total_turnaround_time / n;
    
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    // Free memory allocated for the queue
    free(ready_queue->array);
    free(ready_queue);
}



int main() {
    struct process processes[MAX_PROCESSES];
    int n;
    printf("Enter the number of processes: \n");
    scanf("%d", &n);
    generate_processes(processes, n);              // Generate n random processes
    sort_processes_on_arrival(processes, n);       // Sort the processes by arrival time    
    print_process_table(processes, n);             // Print process table
    run_sjf(processes, n);                         // Run the Shortest Job First scheduling algorithm
    print_table(processes, n);                     // Print the process table
    return 0;                                      // Exit the program
}
