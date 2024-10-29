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
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].turnaround_time = threads[i].waiting_time + threads[i].burst_time;
        threads[i].completion_time = threads[i].turnaround_time + threads[i].arrival_time;
    }
}

void print_results(ThreadData *threads)
{
    /* ToDo3: Implement a function to print the results of the scheduling algorithms.
     * Display the waiting time, turnaround time, completion time, and sum for each thread.
     */
    printf("\nScheduling Results\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("Thread %d: Waiting Time: %d, Turnaround Time: %d, Completion Time: %d, Sum: %d\n",
               threads[i].tid, threads[i].waiting_time, threads[i].turnaround_time, threads[i].completion_time, threads[i].sum);
    }
}

void calculate_sum(ThreadData *thread)
{

    /* ToDo4: Implement the logic to calculate the sum of values in the thread's array.
     * Iterate over the 'array' of the thread and compute the sum.
     *Store the result in the thread's 'sum' field.
     */
    thread->sum = 0;
    for (int i = 0; i < thread->burst_time; i++)
    {
        thread->sum += thread->array[i];
    }
}

void *fcfs_thread(void *arg)
{
    /* ToDo5: Implement the logic to simulate the execution of the thread in FCFS.
     * Use 'compare_and_swap' to mark the thread as running. Calculate its sum, simulate execution using 'usleep', and reset its scheduler state.
     */
    ThreadData *thread = (ThreadData *)arg;
    printf("Thread %d is now running.\n", thread->tid);
    if (compare_and_swap(&thread->scheduler_array[thread->tid], 0, 1))
    {                                             // Try to mark as running
        calculate_sum(thread);                    // Calculate sum to simulate burst time
        usleep(thread->burst_time * 1000);        // Simulate execution (in milliseconds)
        thread->scheduler_array[thread->tid] = 0; // Reset state to ready
    }
    return NULL;
}

void fcfs(ThreadData *threads)
{
    /* ToDo6: Implement the First-Come, First-Served (FCFS) scheduling algorithm.
     * - Assign waiting times based on the order of arrival.
     * - Create and join threads.
     * - Calculate and print the scheduling results.
     */

    // Assign waiting times
    int current_time = 0; // Track the current time for calculating waiting times

    // Calculate waiting times based on the order of arrival
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // Calculate waiting time
        threads[i].waiting_time = current_time - threads[i].arrival_time;

        // Update current time to reflect when this thread finishes execution
        current_time += threads[i].burst_time;
    }

    pthread_t tids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&tids[i], NULL, fcfs_thread, (void *)&threads[i]);
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(tids[i], NULL);
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
    printf("Thread %d is now running.\n", thread->tid);
    if (compare_and_swap(&thread->scheduler_array[thread->tid], 0, 1))
    {                                             // Mark as running
        calculate_sum(thread);                    // Calculate the sum
        usleep(thread->burst_time * 1000);        // Simulate processing time
        thread->scheduler_array[thread->tid] = 0; // Reset state to ready
    }
    return NULL;
}

void sjf(ThreadData *threads)
{
    /* ToDo8: Implement the Shortest Job First (SJF) scheduling algorithm.
     * - Sort the threads based on their burst times.
     * - Assign waiting times, create threads, and join them.
     * - Calculate and print the scheduling results.
     */

    // Sort threads based on arrival time first then by burst time
    for (int i = 0; i < NUM_THREADS - 1; i++)
    {
        for (int j = 0; j < NUM_THREADS - i - 1; j++)
        {
            if (threads[j].arrival_time > threads[j + 1].arrival_time)
            {
                ThreadData temp = threads[j];
                threads[j] = threads[j + 1];
                threads[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < NUM_THREADS - 1; i++)
    {
        for (int j = 1; j < NUM_THREADS - i - 1; j++)
        {
            if (threads[j].burst_time > threads[j + 1].burst_time)
            {
                ThreadData temp = threads[j];
                threads[j] = threads[j + 1];
                threads[j + 1] = temp;
            }
        }
    }

    // Showing the order of threads with their burst time smallest to greatest.
    // for(int i = 0; i < NUM_THREADS; i++){
    //     printf("Thread %d with AT: %d\tBT: %d\n",threads[i].tid, threads[i].arrival_time, threads[i].burst_time);
    // }

    // Assign waiting times considering arrival times
    int current_time = 0; // Initialize current time to 0
    for (int i = 0; i < NUM_THREADS; i++)
    {
        // If the current time is less than the arrival time of the thread, we need to wait
        if (current_time < threads[i].arrival_time)
        {
            current_time = threads[i].arrival_time; // Advance time to the thread's arrival
        }
        threads[i].waiting_time = current_time - threads[i].arrival_time; // Calculate waiting time
        current_time += threads[i].burst_time;                            // Update current time after execution
    }

    // Create and execute each thread
    pthread_t thread_ids[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(&thread_ids[i], NULL, sjf_thread, &threads[i]);
    }

    // Join the threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
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
     * - After all threads have completed, calculate and print the scheduling results.
     */
    if (threads == NULL)
    {
        printf("No threads given.");
        return;
    }

    int completed = 0;    // Number of threads completed
    int current_time = 0; // Current simulation time

    while (completed < NUM_THREADS)
    {
        // Find the thread with the shortest remaining burst time that has already arrived
        int shortest_index = -1;
        int min_burst_time = __INT_MAX__;

        // Loop through all threads to find the shortest burst time within the arrival range
        for (int i = 0; i < NUM_THREADS; i++)
        {
            // Check if the process has arrived and is still pending (remaining_time > 0)
            if (threads[i].arrival_time <= current_time && threads[i].remaining_time > 0)
            {
                // If the current process has a shorter remaining time than the found minimum
                if (threads[i].remaining_time < min_burst_time)
                {
                    min_burst_time = threads[i].remaining_time; // Update the minimum remaining time
                    shortest_index = i;                         // Update the index of the shortest job
                }
            }
        }

        if (shortest_index == -1)
        {
            // No thread is ready to run at the current time, so advance the time.
            current_time++;
            continue;
        }

        // Execute the thread with the shortest remaining time for one unit of time
        printf("Thread %d is now running (Remaining Time: %d)\n", threads[shortest_index].tid, threads[shortest_index].remaining_time);
        threads[shortest_index].remaining_time--;
        current_time++;
        for (int i = 0; i < NUM_THREADS; i++)
        {
            if (threads[shortest_index].tid != threads[i].tid && threads[i].remaining_time > 0)
                threads[i].waiting_time++;
        }

        // If the thread completes its execution
        if (threads[shortest_index].remaining_time == 0)
        {
            threads[shortest_index].completion_time = current_time;
            calculate_sum(&threads[shortest_index]); // Calculate sum after completion
            completed++;
            printf("Thread %d has finished execution\n", threads[shortest_index].tid);
            if (shortest_index != completed - 1) // Avoid unnecessary swap if it's already in place
            {
                // Swap the completed thread with the thread at the `completed - 1` index
                ThreadData temp = threads[completed - 1];
                threads[completed - 1] = threads[shortest_index];
                threads[shortest_index] = temp;

                // Update the `shortest_index` to reflect its new position
                shortest_index = completed - 1;
            }
        }
    }

    calculate_times(threads);
    print_results(threads);
}
