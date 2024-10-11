#include "project2.h"

// Function to initialize the matrix
void initialize_matrix() {
    for (int i = 0; i < ROWS_COUNT; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            matrix[i][j] = i + j;  // Set matrix value as i + j
        }
    }
}

// Thread function to sum rows of the matrix
void *sum_matrix_rows(void *arg) {
    thread_data_t *data = (thread_data_t *)arg;
    long sum = 0;

    /* ToDo1:
     * Sum the assigned rows from start_row to end_row for the current thread.
     * Store the result in partial_sums[data->thread_id].
     */
    // Sum the assigned rows from start_row to end_row for the current thread
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < COLUMN_COUNT; j++) {
            sum += matrix[i][j];
        }
    }
    partial_sums[data->thread_id] = sum;

    pthread_exit(NULL);
}
