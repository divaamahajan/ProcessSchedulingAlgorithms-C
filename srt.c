#include <stdio.h>
#include <limits.h>
#include "process.h"

/**
 * Calculates the waiting time for each process in the given array.
 *
 * @param proc An array of process structs representing the processes to calculate waiting time for.
 * @param n The number of processes in the array.
 */
void findWaitingTime(struct process proc[], int n)
{
  int started[n];
  for (int i = 0; i < n; i++)
  {
    started[i] = 0;
    proc[i].remaining_time = proc[i].expected_burst_time;
  }

  int complete = 0, t = 0, minm = INT_MAX;
  int shortest = 0, finish_time;
  int check = 0;

  while (complete != n)
  {

    // Find process with minimum
    // remaining time among the
    // processes that arrives till the
    // current time`
    for (int j = 0; j < n; j++)
    {
      if ((proc[j].arrival_time <= t) && (proc[j].remaining_time < minm) && proc[j].remaining_time > 0)
      {
        minm = proc[j].remaining_time;
        shortest = j;
        check = 1;
        if (started[j] != 1)
        {
          started[j] = 1;
          proc[j].start_time = t;
        }
      }
    }

    if (check == 0)
    {
      t++;
      continue;
    }
    // print execution
    // print_execution(shortest, t, t + 1);

    // Reduce remaining time by one
    proc[shortest].remaining_time--;

    // Update minimum
    minm = proc[shortest].remaining_time;
    if (minm == 0)
      minm = INT_MAX;

    // If a process gets completely
    // executed
    if (proc[shortest].remaining_time == 0)
    {

      // Increment complete
      complete++;
      check = 0;

      // Find finish time of current
      // process
      finish_time = t + 1;

      // Calculate waiting time
      proc[shortest].waiting_time = finish_time -
                                    proc[shortest].expected_burst_time -
                                    proc[shortest].arrival_time;

      if (proc[shortest].waiting_time < 0)
        proc[shortest].waiting_time = 0;
      proc[shortest].response_time = proc[shortest].start_time - proc[shortest].arrival_time;
    }
    // Increment time
    t++;
  }
}

/**
 * Calculates the turn-around time for each process in the given array.
 *
 * @param proc An array of process structs representing the processes to calculate turn-around time for.
 * @param n The number of processes in the array.
 */
void findTurnAroundTime(struct process proc[], int n)
{
  for (int i = 0; i < n; i++)
    proc[i].turnaround_time = proc[i].expected_burst_time + proc[i].waiting_time;
}

/**
 * Calculates the average waiting time and average turn-around time for the given processes.
 *
 * @param proc An array of process structs representing the processes to calculate average times for.
 * @param n The number of processes in the array.
 */
void findavgTime(struct process proc[], int n)
{
  int total_wt = 0, total_tat = 0;

  // Function to find waiting time of all
  // processes
  findWaitingTime(proc, n);

  // Function to find turn around time for
  // all processes
  findTurnAroundTime(proc, n);

  // Display processes along with all
  // details
  // printf(" P\tBT\tWT\tTAT\n");

  // Calculate total waiting time and
  // total turnaround time
  // for (int i = 0; i < n; i++)
  // {
  //   total_wt = total_wt + proc[i].waiting_time;
  //   total_tat = total_tat + proc[i].turnaround_time;
  //   printf(" %d\t%d\t%d\t%d\t%d\t%d\n", proc[i].id,
  //          proc[i].expected_burst_time, proc[i].waiting_time, proc[i].turnaround_time, proc[i].start_time, proc[i].response_time);
  // }

  // printf("\nAverage waiting time = %f", (float)total_wt / (float)n);
  // printf("\nAverage turn around time = %f", (float)total_tat / (float)n);
}

// Driver code
// int main()
// {
//   struct process proc[] = {
//       {1, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0},
//       {2, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0},
//       {3, 1, 8, 0, 0, 0, 0, 0, 0, 0, 0},
//       {4, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0},
//       {5, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0}};
//   int n = sizeof(proc) / sizeof(proc[0]);

//   findavgTime(proc, n);
//   return 0;
// }