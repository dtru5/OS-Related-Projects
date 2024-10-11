#ifndef PROJECT2_H
#define PROJECT2_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ROWS_COUNT 256
#define COLUMN_COUNT 256
#define MAX_THREADS 256

// Struct to pass thread data
typedef struct {
    int thread_id;
    int start_row;
    int end_row;
} thread_data_t;

// Global variables
long *partial_sums;
pthread_t *threads;
int matrix[ROWS_COUNT][COLUMN_COUNT];

// Function prototypes
void initialize_matrix();
void *sum_matrix_rows(void *arg);

#endif // PROJECT2_H
