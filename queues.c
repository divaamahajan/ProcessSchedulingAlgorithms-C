#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"


// Define the struct for the queue node
struct queue_node {
    int idx;
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

// void enqueue_sorted_burst_time(struct queue* q, struct process* p, int (*compare)(struct process*, struct process*)) {
void enqueue_sorted_burst_time(struct queue* q, struct process* processes, int idx, int (*compare)(struct process*, int, struct process*, int)) {
    // Check if expected burst time is less than 1
    if (processes[idx].expected_burst_time < 1 || processes[idx].id < 1) {
        return;
    }
    // Create a new node
    struct queue_node* new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
    new_node->idx = idx;
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
    while (current_node != NULL && compare(processes, idx, processes + current_node->idx, current_node->idx) >= 0) {        
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
    return;
}


// Function to add a process at the end of the queue
void enqueue(struct queue* q, int idx) {
    // Check if idx is valid
    if (idx < 0) {
        return;
    }
    // Create a new node
    struct queue_node* new_node = (struct queue_node*) malloc(sizeof(struct queue_node));
    new_node->idx = idx;
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
    return;
}


int dequeue(struct queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty.\n");
        return -1;
    }
    // Get the process at the front of the queue
    struct queue_node* front_node = q->front;
    int idx = front_node->idx;
    // Update the front of the queue
    q->front = front_node->next;
    // Free the memory allocated for the front node
    free(front_node);
    // Update the size of the queue
    q->size--;
    return idx;
}


// Function to remove a process from the front of the queue
// Function to remove a process from the front of the queue and update its values
void print_queue(struct queue* q, struct process* processes) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        return;
    }
    struct queue_node* current_node = q->front;
    while (current_node != NULL) {
        printf("P%d", processes[current_node->idx].id);
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