#include "scheduling_algorithms.h"

// Compare-And-Swap function using GCC built-in atomic operation
bool compare_and_swap(int *location, int expected, int new_value) {
     // ToDo1: Implement the compare-and-swap logic using GCC's built-in atomic operation.
    // Hint: We use an atomic operation to prevent race conditions when multiple threads try to update shared data.
    // Use __sync_val_compare_and_swap(location, expected, new_value).
    // Compare the value at 'location' with 'expected'. If they are equal, update 'location' to 'new_value'.
   // Return true if the swap happened, and false otherwise.

}

void calculate_times(ThreadData *threads) {
    // ToDo2: Calculate waiting times, turnaround times, and completion times for each thread.
    // - Waiting time: Time the thread spends waiting before its execution starts.
    // - Turnaround time: Time from arrival to completion (Turnaround time = Waiting time + Burst time).
    // - Completion time: The actual time when the thread completes (Completion time = Turnaround time + Arrival time).

    
}

void print_results(ThreadData *threads) {
    /* ToDo3: Implement a function to print the results of the scheduling algorithms.
     * Display the waiting time, turnaround time, completion time, and sum for each thread.
     */
    
}

void calculate_sum(ThreadData *thread) {

    /* ToDo4: Implement the logic to calculate the sum of values in the thread's array.
     * Iterate over the 'array' of the thread and compute the sum.
     *Store the result in the thread's 'sum' field.
     */
    
}

void *fcfs_thread(void *arg) {
    /* ToDo5: Implement the logic to simulate the execution of the thread in FCFS.
     * Use 'compare_and_swap' to mark the thread as running. Calculate its sum, simulate execution using 'usleep', and reset its scheduler state.
     */
    ThreadData *thread = (ThreadData *)arg;

}

void fcfs(ThreadData *threads) {
    /* ToDo6: Implement the First-Come, First-Served (FCFS) scheduling algorithm.
     * - Assign waiting times based on the order of arrival.
     * - Create and join threads.
     * - Calculate and print the scheduling results.
     */
   
    calculate_times(threads);
    print_results(threads);
}

void *sjf_thread(void *arg) {
    /* ToDo7: Implement the logic to simulate execution of the thread in SJF.
     * Calculate the thread's sum, simulate execution using 'usleep', and print its status.
     */
    ThreadData *thread = (ThreadData *)arg;
    
}

void sjf(ThreadData *threads) {
    /* ToDo8: Implement the Shortest Job First (SJF) scheduling algorithm.
     * - Sort the threads based on their burst times.
     * - Assign waiting times, create threads, and join them.
     * - Calculate and print the scheduling results.
     */
    
    calculate_times(threads);
    print_results(threads);
}

void srjf(ThreadData *threads) {
    /* ToDo9: Implement the Shortest Remaining Job First (SRJF) scheduling algorithm.
     * - Use a time variable and continuously check for the thread with the shortest remaining burst time.
     * - Implement preemption by switching to the thread with the shortest remaining time.
     * - After all threads have completed, calculate and print the scheduling results
     */
    
    calculate_times(threads);
    print_results(threads);
}
