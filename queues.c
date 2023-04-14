#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct process {
    int id                      ;                 
    int arrival_time            ;              
    int expected_burst_time     ;            
    int remaining_time          ;             
    int priority                ;
    int completion_time         ;
    int waiting_time            ;
    int turnaround_time         ;
    int response_time           ;
    int start_time              ;
};

// Define the struct for the queue node
struct queue_node {
    struct process process;
    struct queue_node* next;
};

// Define the struct for the queue
struct queue {
    struct queue_node* front;
    struct queue_node* rear;
    int size;
};

// Function to create a new queue
struct queue* create_queue() {
    struct queue* q = (struct queue*) malloc(sizeof(struct queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

// Function to check if the queue is empty
int is_empty(struct queue* q) {
    return q->size == 0;
}

void enqueue_sorted_burst_time(struct queue* q, struct process p, int (*compare)(const void*, const void*)) {
    // Check if expected burst time is less than 1
    if (p.expected_burst_time < 1 || p.id < 1) {
        return;
    }
    // Create a new node
    struct queue_node* new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
    new_node->process = p;
    new_node->next = NULL;
    // If queue is empty, add the process to the front
    if (is_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
        q->size++;
        return;
    }
    // Find the position to insert the new process
    struct queue_node* current_node = q->front;
    struct queue_node* prev_node = NULL;
    while (current_node != NULL && compare(&p, &current_node->process) >= 0) {        
        prev_node = current_node;
        current_node = current_node->next;
    }
    // Insert the new process
    if (prev_node == NULL) {
        new_node->next = current_node;
        q->front = new_node;
    } else {
        prev_node->next = new_node;
        new_node->next = current_node;
    }
    if (new_node->next == NULL) {
        q->rear = new_node;
    }
    q->size++;
}

// Function to add a process at the end of the queue
void enqueue(struct queue* q, struct process p) {
    // Create a new node
    struct queue_node* new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
    new_node->process = p;
    new_node->next = NULL;
    // If queue is empty, add the process to the front
    if (is_empty(q)) {
        q->front = new_node;
        q->rear = new_node;
        q->size++;
        return;
    }
    // Add the process to the rear
    q->rear->next = new_node;
    q->rear = new_node;
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
    // Remove the process from the front
    struct queue_node* temp_node = q->front;
    struct process p = temp_node->process;
    q->front = temp_node->next;
    free(temp_node);
    q->size--;
    return p;
}

void print_queue(struct queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return;
    }
    struct queue_node* current_node = q->front;
    while (current_node != NULL) {
        printf("P%d", current_node->process.id);
        if (current_node->next != NULL) {
            printf(" -> ");
        }
        current_node = current_node->next;
    }
    printf("\n");
}

void free_queue(struct queue* q) {
    while (!is_empty(q)) {
        dequeue(q);
    }
    free(q);
}