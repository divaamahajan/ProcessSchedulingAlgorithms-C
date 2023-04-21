//
// Created by maximus on 4/20/23.
//

#ifndef PROCESSSCHEDULINGALGORITHMS_C_UTILS_H
#define PROCESSSCHEDULINGALGORITHMS_C_UTILS_H

//
// Created by sreeja on 4/20/23.
//
struct hpf_queue* create_hpf_queue();
int is_hpf_queue_empty(struct hpf_queue* q);
void sort_priority(struct hpf_queue* q,  int (*cmp)(struct process*, struct process*));
void enqueue_hpf(struct hpf_queue* q, struct process* p);
struct process* dequeue_hpf(struct hpf_queue* q);
void free_hpf_queue(struct hpf_queue* q);
int compare_priority(struct process* p1 , struct process* p2);
void print_exec(int id);
void calculate_times_process(struct process* p, int current_time);
void calculate_statistics_pq(struct hpf_queue *final_queue, int queue_no);


#endif //PROCESSSCHEDULINGALGORITHMS_C_UTILS_H
