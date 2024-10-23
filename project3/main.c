// main.c

#include "scheduling_algorithms.h"

int main() {
    int scheduler_array[NUM_THREADS] = {0, 0, 0, 0}; // Scheduler array to track thread state

    //  threads with  burst times and  arrival times
    ThreadData threads[NUM_THREADS] = {
        {0, 15, 15, 0, 0, 0, 0, (int[]){1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 0, scheduler_array}, // Thread 0, burst_time 15, arrival_time 0
        {1, 5, 5, 0, 0, 0, 3, (int[]){16, 17, 18, 19, 20}, 0, scheduler_array},          // Thread 1, burst_time 5, arrival_time 3
        {2, 10, 10, 0, 0, 0, 5, (int[]){21, 22, 23, 24, 25, 26, 27, 28, 29, 30}, 0, scheduler_array},  // Thread 2, burst_time 10, arrival_time 5
        {3, 7, 7, 0, 0, 0, 2, (int[]){31, 32, 33, 34, 35, 36, 37}, 0, scheduler_array}    // Thread 3, burst_time 7, arrival_time 2
    };

    printf("Running FCFS Scheduling:\n");
    fcfs(threads);

    // Reset thread data for the next scheduling algorithm
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].waiting_time = 0;
        threads[i].turnaround_time = 0;
        threads[i].remaining_time = threads[i].burst_time;
        threads[i].sum = 0;
    }

    printf("\nRunning SJF Scheduling:\n");
    sjf(threads);

    // Reset thread data for the next scheduling algorithm
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].waiting_time = 0;
        threads[i].turnaround_time = 0;
        threads[i].remaining_time = threads[i].burst_time;
        threads[i].sum = 0;
    }

    printf("\nRunning SRJF Scheduling:\n");
    srjf(threads);

    return 0;
}
