#include "project2.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    // Added a check for using only powers of 2
    if ((num_threads % 2 != 0 && num_threads != 1)|| num_threads <= 0 || num_threads > MAX_THREADS)
    {
        printf("Invalid number of threads: %d\n", num_threads);
        return 1;
    }

    struct timespec start, end;
    long long seconds, nano_seconds;

    // Allocate memory for partial sums and threads
    partial_sums = (long *)malloc(num_threads * sizeof(long));
    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    thread_data_t *thread_data = (thread_data_t *)malloc(num_threads * sizeof(thread_data_t));

    // Fixed array for each thread's data set to the size of num_threads
    //thread_data_t thread_data[num_threads];

    // Initialize the matrix
    initialize_matrix();

    clock_gettime(CLOCK_REALTIME, &start);

    // Divide rows among threads
    int rows_per_thread = ROWS_COUNT / num_threads;

    /* ToDo1:
     * Create num_threads threads to calculate the partial sums.
     * Each thread should process rows starting from `start_row` to `end_row`.
     * The sum of each thread's assigned rows should be stored in `partial_sums`.
     */
    for(int i = 0; i < num_threads; i++){
        // Initialize each i thread data
        thread_data[i].thread_id = i;
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i + 1) * rows_per_thread;

        //pthread_create(&threads[i], NULL, sum_matrix_rows, &thread_data[i]);
        pthread_create(&threads[i], NULL, sum_matrix_rows, (void *)&thread_data[i]);
    }



    /* ToDo2:
     * Wait for all threads to complete execution.
     */
    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
    }

    /* ToDo3:
     * Sum all the partial sums computed by the threads and print the total sum.
     */
    long total_sum = 0;
    for(int i = 0; i < num_threads; i++){
        total_sum += partial_sums[i];
    }
    printf("Total sum: %ld\n", total_sum);

    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate execution time
    /* ToDo4:
     * Calculate the time required and print.
     */
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double execution_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d thread(s): %Lf seconds\n", num_threads, execution_time);

    // Free memory
    free(partial_sums);
    free(threads);

    return 0;
}
