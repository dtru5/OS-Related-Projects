#include "project1.h"

int main(int argc, char *argv[]) {

    if (argc != 6) {
        printf("Usage: %s <starting_row_number> <number_of_rows_to_sum> shmid_matrix shmid_partial_sum partial_sum_index\n", argv[0]);
        return 1;
    }

    int start_row_id = atoi(argv[1]);
    int end_row_id = atoi(argv[2]);
    int shmid_matrix = atoi(argv[3]);
    int shmid_partial_sum = atoi(argv[4]);
    int partial_sum_index = atoi(argv[5]);



    if ((start_row_id < 0) ||(start_row_id>=ROWS_COUNT)) {
        printf("Invalid starting row id: %d\n", start_row_id);
        return 1;
    }


    /* ToDo1:
     * Complete the body of the child_proc.c
     * Hints:
     *  1) First attach both memory regions
     *  2) use sum_matrix() function to calllculate sume of
     *  all elements from start_row to end_row
     *  3) Ensure to detach both memory regions before leaving the main()
     * Attach memory region 1
    */

    int* matrix = shmat(shmid_matrix, NULL, 0);
    int* partial_sum = shmat(shmid_partial_sum, NULL, 0);
   
    long sum = sum_matrix(start_row_id, end_row_id, matrix);
    partial_sum[partial_sum_index] = sum;

    shmdt(matrix);
    shmdt(partial_sum);

    exit(0);
}
