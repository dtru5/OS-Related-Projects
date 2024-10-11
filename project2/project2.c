#include "project2.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0 || num_threads > MAX_THREADS) {
        printf("Invalid number of threads: %d\n", num_threads);
        return 1;
    }

    struct timespec start, end;
    long long seconds, nano_seconds;

    // Allocate memory for partial sums and threads
    partial_sums = (long *)malloc(num_threads * sizeof(long));
    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

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

    /* ToDo2:
     * Wait for all threads to complete execution.
     */

    /* ToDo3:
     * Sum all the partial sums computed by the threads and print the total sum.
    */

    clock_gettime(CLOCK_REALTIME, &end);

    // Calculate execution time
    /* ToDo4:
     * Calculate the time required and print.
    */
    printf("Execution Time with %d thread(s): %Lf seconds\n", num_threads, execution_time);

    // Free memory
    free(partial_sums);
    free(threads);

    return 0;
}
