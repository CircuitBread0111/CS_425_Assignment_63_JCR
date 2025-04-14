///////////////////////////|
//|File: pth_multiplication.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0.0
//|Date: April 14, 2025
///////////////////////////|

/* Descriptiion:
* This program demonstrates the use of pthreads to create multiple threads
* that compute the product of two matrices. The program takes the number of threads
* as a command line argument. Each thread computes the product of a portion of the
* matrices, and the main thread combines the results to get the final product.
* The program also measures the time taken to compute the product using multiple
* threads and compares it with the expected product using a serial implementation.
* The expected product is calculated using a nested loop to multiply the matrices.
* The program also validates the result by comparing the parallel and serial results.
*/

//----------------------------------------------------------------

// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MATRIX_SIZE 1000

// Struct //
typedef struct {
    int *matrix1;
    int *matrix2;
    int *result;
    int start_row;
    int end_row;
} ThreadData;

// Multiply //
void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;    // Cast the argument to ThreadData
    for (int i = data->start_row; i < data->end_row; i++) { // Iterate over rows
        for (int j = 0; j < MATRIX_SIZE; j++) {   // Iterate over columns
            int sum = 0;
            for (int k = 0; k < MATRIX_SIZE; k++) { // Iterate over elements
                sum += data->matrix1[i * MATRIX_SIZE + k] * data->matrix2[k * MATRIX_SIZE + j];     // Multiply
            }
            data->result[i * MATRIX_SIZE + j] = sum;    // Store the result
        }
    }
    return NULL;
}

// Main //
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);    // Get the number of threads from command line argument
    if (num_threads <= 0) {     // Check if the number of threads is valid
        fprintf(stderr, "Number of threads must be positive.\n");
        return 1;
    }

    int *matrix1 = malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE); // Allocate memory for the first matrix
    int *matrix2 = malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE); // Allocate memory for the second matrix
    int *serial_result = malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE);   // Allocate memory for the serial result
    int *parallel_result = malloc(sizeof(int) * MATRIX_SIZE * MATRIX_SIZE); // Allocate memory for the parallel result
    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);   // Allocate memory for thread IDs
    ThreadData* thread_data = malloc(sizeof(ThreadData) * num_threads); // Allocate memory for thread data

    srand(time(NULL));  // Seed the random number generator
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {   // Initialize matrices with random values
        matrix1[i] = rand() % 1000;  //  Random values between 0 and 999
        matrix2[i] = rand() % 1000;  //  Random values between 0 and 999
        serial_result[i] = 0;       // Initialize serial result to 0
        parallel_result[i] = 0;     // Initialize parallel result to 0
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);     // Get the start time

    // Serial //
    for (int i = 0; i < MATRIX_SIZE; i++) {     // Serial multiplication
        for (int j = 0; j < MATRIX_SIZE; j++) {     // Iterate over rows and columns
            for (int k = 0; k < MATRIX_SIZE; k++) {     // Iterate over elements
                serial_result[i * MATRIX_SIZE + j] += matrix1[i * MATRIX_SIZE + k] * matrix2[k * MATRIX_SIZE + j];
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double serial_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Serial Time: %.4f seconds\n", serial_time);

    // Parallel //
    int base_rows = MATRIX_SIZE / num_threads;
    int remainder = MATRIX_SIZE % num_threads;
    int current_row = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < num_threads; i++) { // Create threads
        int rows = base_rows + (i < remainder ? 1 : 0); // Adjust row count for remainder
        thread_data[i] = (ThreadData){  // Initialize thread data
            matrix1, matrix2, parallel_result,  //  Matrix pointers
            current_row,
            current_row + rows
        };
        pthread_create(&threads[i], NULL, multiply, &thread_data[i]);       // Create thread
        current_row += rows;
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL); // Wait for each thread to finish
    }

    clock_gettime(CLOCK_MONOTONIC, &end);
    double parallel_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    // Validation //
    int correct = 1;
    for (int i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++) {   // Compare results
        if (serial_result[i] != parallel_result[i]) {   //  Mismatch found
            correct = 0;
            break;
        }
    }

    printf("Parallel Time: %.4f seconds\n", parallel_time);
    printf("Speedup: %.2fx\n", serial_time / parallel_time);
    printf("Result: %s\n", correct ? "Match" : "Mismatch!");

    free(matrix1);      // Free the allocated memory for the first matrix
    free(matrix2);          // Free the allocated memory for the second matrix
    free(serial_result);    // Free the allocated memory for the serial result
    free(parallel_result);  // Free the allocated memory for the parallel result
    free(threads);          // Free the allocated memory for thread IDs
    free(thread_data);      // Free the allocated memory for thread data
    return 0;
}
