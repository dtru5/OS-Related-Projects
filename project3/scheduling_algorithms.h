// scheduling_algorithms.h

#ifndef SCHEDULING_ALGORITHMS_H
#define SCHEDULING_ALGORITHMS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define NUM_THREADS 4 // Number of threads

typedef struct {
    int tid;                // Thread ID
    int burst_time;         // Total CPU burst time (array length)
    int remaining_time;     // Remaining CPU burst time
    int waiting_time;       // Waiting time
    int turnaround_time;    // Turnaround time
    int completion_time;    // Completion time
    int arrival_time;       // Arrival time
    int *array;             // Pointer to the thread's array of integers
    int sum;                // Result of sum calculation
    int *scheduler_array;   // Reference to scheduler array
} ThreadData;

// Function declarations
bool compare_and_swap(int *location, int expected, int new_value);
void calculate_times(ThreadData *threads);
void print_results(ThreadData *threads);
void calculate_sum(ThreadData *thread);
void *fcfs_thread(void *arg);
void fcfs(ThreadData *threads);
void *sjf_thread(void *arg);
void sjf(ThreadData *threads);
void srjf(ThreadData *threads);

#endif // SCHEDULING_ALGORITHMS_H
