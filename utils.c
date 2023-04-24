//
// Created by sreeja on 4/20/23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"

struct node {
    struct process* p;
    struct node* next;
};


int compare_priority(struct process* p1 , struct process* p2) {
    if ((p1->priority == p2->priority) && (p1->arrival_time > p2->arrival_time)) {
        return 1;
    }
    else if (p1->priority > p2->priority) {
        return -1;
    }
    else{
        return 1;
    }
}

void print_exec(int id) {
    char str1[12]; // increase buffer size to 12
    if (id < 0) {
        return;
//        printf("-\t");
    } else {
        printf("P%d ", id);
    }
}

void calculate_times_process(struct process* p, int current_time) {
    p->completion_time = current_time + p->expected_burst_time;
    p->turnaround_time = p->completion_time - p->arrival_time;
    p->waiting_time = p->turnaround_time - p->expected_burst_time;
    p->response_time = p->start_time - p->arrival_time;
}

struct hpf_queue {
    struct node* front;
    struct node* rear;
    int size;
};

struct hpf_queue* create_hpf_queue() {
    struct hpf_queue* q = (struct hpf_queue*) malloc(sizeof(struct hpf_queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int is_hpf_queue_empty(struct hpf_queue* q) {
    return q->size == 0;
}

void sort_priority(struct hpf_queue* q,  int (*cmp)(struct process*, struct process*)) {
    struct node* current_node = q->front;
    while (current_node != NULL) {
        struct node* next_node = current_node->next;
        while (next_node != NULL) {
            struct process *p1 = current_node->p;
            struct process *p2 = next_node->p;
            if ((*cmp)(p1, p2) > 0) {
                struct process* temp = p1;
                p1 = p2;
                p2 = temp;
            }
            next_node = next_node->next;
        }
        current_node = current_node->next;
    }
}

void enqueue_hpf(struct hpf_queue* q, struct process* p) {
    // Check if idx is valid
    if (p == NULL) {
        return;
    }
    // Create a new node
    struct node* new_node = (struct node*) malloc(sizeof(struct node));
    new_node->p = p;
    new_node->next = NULL;
    // If queue is empty, add the process to the front
    if (is_hpf_queue_empty(q)) {
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

struct process* dequeue_hpf(struct hpf_queue* q) {
    // If queue is empty, return NULL
    if (is_hpf_queue_empty(q)) {
        return NULL;
    }
    // Remove the process from the front
    struct node* front_node = q->front;
    struct process* p = front_node->p;
    q->front = front_node->next;
    q->size--;
    free(front_node);
    return p;
}

void free_hpf_queue(struct hpf_queue* q) {
    while (!is_hpf_queue_empty(q)) {
        dequeue_hpf(q);
    }
    free(q);
}


void calculate_statistics_pq(struct hpf_queue *final_queue, int queue_no)
{
    float total_turnaround_time = 0;
    float total_waiting_time = 0;
    float total_response_time = 0;
    float total_cpu_time = 0;
    float avg_turnaround_time = 0.0;
    float avg_waiting_time = 0.0;
    float avg_response_time = 0.0;
    float avg_throughput = 0.0;

    int n = final_queue->size;
    struct node *current_node = final_queue->front;
    while (current_node != NULL) {
        struct process *p = current_node->p;
        total_turnaround_time += p->turnaround_time;
        total_waiting_time += p->waiting_time;
        total_response_time += p->response_time;
        total_cpu_time += p->expected_burst_time;
        current_node = current_node->next;
    }
    avg_turnaround_time = total_turnaround_time / n;
    avg_waiting_time = total_waiting_time / n;
    avg_response_time = total_response_time / n;
    avg_throughput = (float)n / total_cpu_time;

    printf("\nStatistics for Priority Queue:%d\n", queue_no);
    // Print the calculated statistics
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Response Time: %.2f\n", avg_response_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Throughput: %.2f\n", avg_throughput);
}