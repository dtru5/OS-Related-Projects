#include "project1.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        printf("Invalid number of processes: %d\n", num_processes);
        return 1;
    }


    struct timespec start, end;
    long long seconds, nano_seconds;


    /* ToDo1
     * create and attach shared memory region 1 to store the matrix
     */

    /*  NOTES:
        int shmget(key_t __key, size_t __size, int __shmflg) 
        -System call used for creating and attaching shared memory segments.
        -IPC_PRIVATE makes sure that a new private memory segment is created and can't be accessed by other processes unless
            they are passed through by fork()
        -The size of the memory region is M x N x sizeof(int) since it is M x N size and we need to account for the 4 bytes needed for each int
        -IPC_CREAT is used to create a new memory segment if it does not exist already.
        -0666 specifies read and write permissions for the memory segment
        -Returns a shmid (shared memory identifier) which is used for attaching and controlling the memory segment
    */
    int shm_matrix_id = shmget(IPC_PRIVATE, sizeof(int) * ROWS_COUNT * COLUMN_COUNT, IPC_CREAT | 0666);
    //If unable to allocate memory, print error and exit.
    if (shm_matrix_id < 0){
        perror("shmget error for matrix allocation");
        exit(1);
    }
    /*
        void* shmat(int shmid, const void *shmaddr, int shmflg)
        -Used to attach the shared memory segment created by shmget
        -shmid is the ID we got from using shmget
        -shmaddr is to specify the address where the shared memory should be attached.
            NULL will let the system automatically pick an appropriate address.
        -shmflg is a flag that controls how the memory is attached. 
            When set to 0, the shared memory segment attached in read and write mode.
    */
   int* matrix = shmat(shm_matrix_id, NULL, 0);

    /* ToDo2
     * create and attach shared memory region 2 to store the partial sums that
     * are returned by children processes.
     */
    //Create shared memory segment for partial sum
    int shm_partial_sum_id = shmget(IPC_PRIVATE, sizeof(long) * num_processes, IPC_CREAT | 0666);
    //If unable to create, print error and exit
    if(shm_partial_sum_id < 0){
        perror("shmget error for partial sum allocation");
        exit(1);
    }
    //Attach shared memory segment
    long* partial_sum = (long*) shmat(shm_partial_sum_id, NULL, 0);



    /* ToDo3
     * Initialize the shared memory region 1 to hold
     * the matrix of size ROW_COUNT x COLUMN_COUNT.
     * Value of each element is set to i+j, where i is index of row
     * and j is index of column.
    */
    for(int i = 0; i < ROWS_COUNT; i++){
        for(int j = 0; j < COLUMN_COUNT; j++){
            /* 'i * COLUMN_COUNT + j' used to iterate through the array as if it was a matrix 
            [0, 1, 2,
             3, 4, 5,
             6, 7, 8] 
            If an array, arr[3] = 3. But for a matrix i = 1, j = 0, so 1 * 3 + 0 = 3 and 
                corresponds to our array (shared memory segment).
            */
            matrix[i * COLUMN_COUNT + j] = i + j;
        }
    }


    clock_gettime(CLOCK_REALTIME, &start);


    /* ToDo4:
     * 1) Create num_processes count of children processes. (using fork)
     * 2) Specify for each process the start_row, end_row, and
     * index in shared memory region 2 where a child process is supposed to store its
     * calculated partial sum.
     *
     * Hints:
     *
     * Google "man fork" and "man execlp"
     *
    */
    for(int i = 0; i < num_processes; i++){
        int start_row = i * (ROWS_COUNT / num_processes);
        int end_row = (i + 1) * (ROWS_COUNT / num_processes) - 1;

        if(fork() == 0){
            char start_row_str[10];
            char end_row_str[10];
            char shm_matrix_str[10];
            char shm_partial_sum_str[10];
            char partial_sum_index_str[10];

            snprintf(start_row_str, 10, "%d", start_row);
            snprintf(end_row_str, 10, "%d", end_row);
            snprintf(shm_matrix_str, 10, "%d", shm_matrix_id);
            snprintf(shm_partial_sum_str, 10, "%d", shm_partial_sum_id);
            snprintf(partial_sum_index_str, 10, "%d", i);

            execlp("./child_proc", "./child_proc", start_row_str, end_row_str, shm_matrix_str, shm_partial_sum_str, partial_sum_index_str, NULL);
            perror("execlp");
            exit(1);
        }
    }



    /* ToDo5
     * Wait for all children processes to complete
    */
    for (int i = 0; i < num_processes; i++) {
        wait(NULL);
    }



    /* ToDo6:
     * Sum all the partial-sums stored in sharred memory region 2 (by children processes)
     * and print the sum.
    */
    long total_sum = 0;
    for(int i = 0; i < num_processes; i++){
        total_sum += partial_sum[i];
    }
    printf("Total Sum: %ld\n", total_sum);


    clock_gettime(CLOCK_REALTIME, &end);
    // Calculate execution time
    seconds=end.tv_sec - start.tv_sec;
    nano_seconds=end.tv_nsec - start.tv_nsec;
    long double executio_time=(long double)seconds+((long double)nano_seconds/1000000000);
    printf("Execution Time with %d process(es): %Lf\n",num_processes,executio_time);


    /* ToDo7:
     * Detach and destroy both shared memory regions.
     * Hint:
     * Google "man shmdt" and "man shmctl"
    */
    shmdt(matrix); //Detach matrix
    shmdt(partial_sum); //Detach partial_sum
    shmctl(shm_matrix_id, IPC_RMID, NULL); //Remove memory region for matrix
    shmctl(shm_partial_sum_id, IPC_RMID, NULL); //Remove memory region for partial sum

    return 0;
}


