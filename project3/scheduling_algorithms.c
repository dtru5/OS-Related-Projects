#include "scheduling_algorithms.h"

// Compare-And-Swap function using GCC built-in atomic operation
bool compare_and_swap(int *location, int expected, int new_value)
{
    // ToDo1: Implement the compare-and-swap logic using GCC's built-in atomic operation.
    // Hint: We use an atomic operation to prevent race conditions when multiple threads try to update shared data.
    // Use __sync_val_compare_and_swap(location, expected, new_value).
    // Compare the value at 'location' with 'expected'. If they are equal, update 'location' to 'new_value'.
    // Return true if the swap happened, and false otherwise.
    return __sync_val_compare_and_swap(location, expected, new_value) == expected;
}

void calculate_times(ThreadData *threads)
{
    // ToDo2: Calculate waiting times, turnaround times, and completion times for each thread.
    // - Waiting time: Time the thread spends waiting before its execution starts.
    // - Turnaround time: Time from arrival to completion (Turnaround time = Waiting time + Burst time).
    // - Completion time: The actual time when the thread completes (Completion time = Turnaround time + Arrival time).
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].turnaround_time = threads[i].waiting_time + threads[i].burst_time;
        threads[i].completion_time = threads[i].turnaround_time + threads[i].arrival_time;
    }
}

void print_results(ThreadData *threads)
{
    /* ToDo3: Implement a function to print the results of the scheduling algorithms.
     * Display the waiting time, turnaround time, completion time, and sum for each thread.
     */
    printf("Thread ID | Waiting Time | Turnaround Time | Completion Time | Sum\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("   %d      |      %d      |       %d        |       %d        | %d\n",
               threads[i].tid,
               threads[i].waiting_time,
               threads[i].turnaround_time,
               threads[i].completion_time,
               threads[i].sum);
    }
}

void calculate_sum(ThreadData *thread)
{

    /* ToDo4: Implement the logic to calculate the sum of values in the thread's array.
     * Iterate over the 'array' of the thread and compute the sum.
     *Store the result in the thread's 'sum' field.
     */
    thread->sum = 0;
    for (int i = 0; i < thread->burst_time; i++) {
        thread->sum += thread->array[i];
    }
}

void *fcfs_thread(void *arg)
{
    /* ToDo5: Implement the logic to simulate the execution of the thread in FCFS.
     * Use 'compare_and_swap' to mark the thread as running. Calculate its sum, simulate execution using 'usleep', and reset its scheduler state.
     */
    ThreadData *thread = (ThreadData *)arg;

    // Simulate execution
    compare_and_swap(&thread->scheduler_array[thread->tid], 0, 1); // Mark as running
    usleep(thread->burst_time * 1000); // Simulate processing time
    calculate_sum(thread); // Calculate the sum

    // Mark as done
    compare_and_swap(&thread->scheduler_array[thread->tid], 1, 2); // Mark as completed
    return NULL;
}

void fcfs(ThreadData *threads)
{
    /* ToDo6: Implement the First-Come, First-Served (FCFS) scheduling algorithm.
     * - Assign waiting times based on the order of arrival.
     * - Create and join threads.
     * - Calculate and print the scheduling results.
     */

        // Calculate waiting times
    threads[0].waiting_time = 0; // First thread waits for 0 time
    for (int i = 1; i < NUM_THREADS; i++) {
        threads[i].waiting_time = threads[i-1].waiting_time + threads[i-1].burst_time;
    }

    pthread_t thread_ids[NUM_THREADS];

    // Create and join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_ids[i], NULL, fcfs_thread, (void *)&threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    calculate_times(threads);
    print_results(threads);
}

void *sjf_thread(void *arg)
{
    /* ToDo7: Implement the logic to simulate execution of the thread in SJF.
     * Calculate the thread's sum, simulate execution using 'usleep', and print its status.
     */
    ThreadData *thread = (ThreadData *)arg;

    compare_and_swap(&thread->scheduler_array[thread->tid], 0, 1); // Mark as running
    usleep(thread->burst_time * 1000); // Simulate processing time
    calculate_sum(thread); // Calculate the sum
    compare_and_swap(&thread->scheduler_array[thread->tid], 1, 2); // Mark as completed
    return NULL;
}

void sjf(ThreadData *threads)
{
    /* ToDo8: Implement the Shortest Job First (SJF) scheduling algorithm.
     * - Sort the threads based on their burst times.
     * - Assign waiting times, create threads, and join them.
     * - Calculate and print the scheduling results.
     */

    // Sort threads by burst time
    for (int i = 0; i < NUM_THREADS - 1; i++) {
        for (int j = i + 1; j < NUM_THREADS; j++) {
            if (threads[i].burst_time > threads[j].burst_time) {
                ThreadData temp = threads[i];
                threads[i] = threads[j];
                threads[j] = temp;
            }
        }
    }

    // Calculate waiting times for SJF
    threads[0].waiting_time = 0; // First thread waits for 0 time
    for (int i = 1; i < NUM_THREADS; i++) {
        threads[i].waiting_time = threads[i-1].waiting_time + threads[i-1].burst_time;
    }

    pthread_t thread_ids[NUM_THREADS];

    // Create and join threads
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&thread_ids[i], NULL, sjf_thread, (void *)&threads[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(thread_ids[i], NULL);
    }

    calculate_times(threads);
    print_results(threads);
}

void srjf(ThreadData *threads)
{
    /* ToDo9: Implement the Shortest Remaining Job First (SRJF) scheduling algorithm.
     * - Use a time variable and continuously check for the thread with the shortest remaining burst time.
     * - Implement preemption by switching to the thread with the shortest remaining time.
     * - After all threads have completed, calculate and print the scheduling results
     */

    // Initialize remaining times
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].remaining_time = threads[i].burst_time;
    }

    int completed = 0, current_time = 0;
    while (completed < NUM_THREADS) {
        int min_index = -1;

        // Find thread with the shortest remaining time that has arrived
        for (int i = 0; i < NUM_THREADS; i++) {
            if (threads[i].arrival_time <= current_time && threads[i].remaining_time > 0) {
                if (min_index == -1 || threads[i].remaining_time < threads[min_index].remaining_time) {
                    min_index = i;
                }
            }
        }

        if (min_index != -1) {
            threads[min_index].waiting_time = current_time - threads[min_index].arrival_time;
            compare_and_swap(&threads[min_index].scheduler_array[min_index], 0, 1); // Mark as running
            usleep(threads[min_index].remaining_time * 1000); // Simulate processing time
            calculate_sum(&threads[min_index]); // Calculate the sum

            // Mark as completed
            threads[min_index].remaining_time = 0; // Thread completed
            compare_and_swap(&threads[min_index].scheduler_array[min_index], 1, 2); // Mark as completed
            completed++;
            current_time += threads[min_index].burst_time; // Move the time forward
        } else {
            current_time++; // If no threads are ready, move time forward
        }
    }

    calculate_times(threads);
    print_results(threads);
}
