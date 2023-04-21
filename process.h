//
// Created by maximus on 4/20/23.
//

#ifndef PROCESSSCHEDULINGALGORITHMS_C_PROCESS_H
#define PROCESSSCHEDULINGALGORITHMS_C_PROCESS_H

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
    int elapsed_time            ;
};

//typedef struct process process;

#endif //PROCESSSCHEDULINGALGORITHMS_C_PROCESS_H
