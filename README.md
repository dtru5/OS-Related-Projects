OS Projects: Processes, Threading, and Scheduling

This repository contains three operating systems projects that explore foundational concepts including inter-process communication (IPC), multithreading, and CPU scheduling algorithms. Each project demonstrates a specific aspect of concurrent programming and performance evaluation.
Project 1: Matrix Summation Using Processes and Shared Memory

Objective:
Divide the task of summing a 2D matrix across multiple child processes using shared memory and process forking.

Key Features:

    Initializes a shared memory segment to hold a 256×256 matrix.

    Uses a second shared memory segment to store partial sums from each child process.

    Spawns n child processes using fork(), each responsible for summing a subset of matrix rows.

    Each child process executes an external helper program (child_proc) using execlp().

    The parent waits for all children to finish, aggregates the partial sums, and prints the final result along with execution time.

    Properly detaches and deallocates all shared memory segments on completion.

Project 2: Matrix Summation Using Multithreading

Objective:
Achieve matrix summation similar to Project 1, but using POSIX threads instead of processes.

Key Features:

    Divides a 256×256 matrix among n threads (where n is a power of 2 or 1).

    Uses pthread_create to spawn threads that each compute a partial sum.

    Partial results are stored in a shared array allocated on the heap.

    Waits for all threads to complete via pthread_join.

    Aggregates all partial sums and reports the total along with execution time.

    Demonstrates performance improvements due to lighter-weight threading compared to process-based concurrency.

Project 3: CPU Scheduling Simulation

Objective:
Simulate CPU scheduling algorithms and evaluate turnaround and waiting times for a fixed set of threads.

Key Features:

    Models threads with attributes such as arrival time and burst time.

    Implements scheduling strategies (e.g., FCFS, SJF, Round Robin) within scheduling_algorithms.c/h.

    Tracks thread state with a scheduler array.

    Outputs metrics such as average turnaround and waiting time to assess scheduler performance.

Build and Run Instructions

Each project comes with its own Makefile or build instructions. To compile and run:

# Project 1
cd project1
make
./project1 <num_processes>

# Project 2
cd project2
make
./project2 <num_threads>

# Project 3
cd project3
make
./main

Topics Covered

    Shared Memory (SysV IPC)

    Process Control (fork, exec, wait)

    POSIX Threads (pthread_create, pthread_join)

    Scheduling Algorithms

    Performance Timing (clock_gettime)

    Memory Management and Cleanup
