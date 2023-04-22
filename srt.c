#include <stdio.h>
#include <limits.h>
#include "process.h"

/**
 * Calculates the waiting time for each process in the given array.
 *
 * @param proc An array of process structs representing the processes to calculate waiting time for.
 * @param n The number of processes in the array.
 * @param wt An array to store the calculated waiting times in. Must have size at least n.
 */
void findWaitingTime(struct process proc[], int n, int wt[])
{
  int rt[n];

  // Copy the burst time into rt[]
  for (int i = 0; i < n; i++)
    rt[i] = proc[i].expected_burst_time;

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
      if ((proc[j].arrival_time <= t) &&
          (rt[j] < minm) && rt[j] > 0)
      {
        minm = rt[j];
        shortest = j;
        check = 1;
      }
    }

    if (check == 0)
    {
      t++;
      continue;
    }

    // Reduce remaining time by one
    rt[shortest]--;

    // Update minimum
    minm = rt[shortest];
    if (minm == 0)
      minm = INT_MAX;

    // If a process gets completely
    // executed
    if (rt[shortest] == 0)
    {

      // Increment complete
      complete++;
      check = 0;

      // Find finish time of current
      // process
      finish_time = t + 1;

      // Calculate waiting time
      wt[shortest] = finish_time -
                     proc[shortest].expected_burst_time -
                     proc[shortest].arrival_time;

      if (wt[shortest] < 0)
        wt[shortest] = 0;
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
 * @param wt An array of the waiting times for each process, as previously calculated by `findWaitingTime()`.
 * @param tat An array to store the calculated turn-around times in. Must have size at least n.
 */
void findTurnAroundTime(struct process proc[], int n, int wt[], int tat[])
{
  for (int i = 0; i < n; i++)
    tat[i] = proc[i].expected_burst_time + wt[i];
}

/**
 * Calculates the average waiting time and average turn-around time for the given processes.
 *
 * @param proc An array of process structs representing the processes to calculate average times for.
 * @param n The number of processes in the array.
 * @param wt An array of the waiting times for each process, as previously calculated by `findWaitingTime()`.
 * @param tat An array of the turn-around times for each process, as previously calculated by `findTurnAroundTime()`.
 * @param avg_wt A pointer to store the calculated average waiting time.
 * @param avg_tat A pointer to store the calculated average turn-around time.
 */
void findavgTime(struct process proc[], int n)
{
  int wt[n], tat[n], total_wt = 0, total_tat = 0;

  // Function to find waiting time of all
  // processes
  findWaitingTime(proc, n, wt);

  // Function to find turn around time for
  // all processes
  findTurnAroundTime(proc, n, wt, tat);

  // Display processes along with all
  // details
  printf(" P\tBT\tWT\tTAT\n");

  // Calculate total waiting time and
  // total turnaround time
  for (int i = 0; i < n; i++)
  {
    total_wt = total_wt + wt[i];
    total_tat = total_tat + tat[i];
    printf(" %d\t%d\t%d\t%d\n", proc[i].id,
           proc[i].expected_burst_time, wt[i], tat[i]);
  }

  printf("\nAverage waiting time = %f", (float)total_wt / (float)n);
  printf("\nAverage turn around time = %f", (float)total_tat / (float)n);
}

// Driver code
int main()
{
  struct process proc[] = {
      {1, 0, 6, 6, 3, 0, 0, 0, 0, 0, 0},
      {2, 5, 2, 2, 1, 0, 0, 0, 0, 0, 0},
      {3, 1, 8, 8, 2, 0, 0, 0, 0, 0, 0},
      {4, 0, 3, 3, 4, 0, 0, 0, 0, 0, 0},
      {5, 4, 4, 4, 5, 0, 0, 0, 0, 0, 0}};
  int n = sizeof(proc) / sizeof(proc[0]);

  findavgTime(proc, n);
  return 0;
}