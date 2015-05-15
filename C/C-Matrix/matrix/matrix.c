#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <inttypes.h>

#include "matrix.h"

static uint32_t g_seed = 0;

static ssize_t g_width = 0;
static ssize_t g_height = 0;
static ssize_t g_elements = 0;

static ssize_t g_nthreads = 1;

////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Returns pseudorandom number determined by the seed
 */
uint32_t fast_rand(void) {

    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

/**
 * Sets the seed used when generating pseudorandom numbers
 */
void set_seed(uint32_t seed) {

    g_seed = seed;
}

/**
 * Sets the number of threads available
 */
void set_nthreads(ssize_t count) {

    g_nthreads = count;
}

/**
 * Sets the dimensions of the matrix
 */
void set_dimensions(ssize_t order) {

    g_width = order;
    g_height = order;

    g_elements = g_width * g_height;
}

/**
 * Displays given matrix
 */
void display(const uint32_t* matrix) {

    for (ssize_t y = 0; y < g_height; y++) {
        for (ssize_t x = 0; x < g_width; x++) {
            if (x > 0) printf(" ");
            printf("%" PRIu32, matrix[y * g_width + x]);
        }

        printf("\n");
    }
}

/**
 * Displays given matrix row
 */
void display_row(const uint32_t* matrix, ssize_t row) {

    for (ssize_t x = 0; x < g_width; x++) {
        if (x > 0) printf(" ");
        printf("%" PRIu32, matrix[row * g_width + x]);
    }

    printf("\n");
}

/**
 * Displays given matrix column
 */
void display_column(const uint32_t* matrix, ssize_t column) {

    for (ssize_t y = 0; y < g_height; y++) {
        printf("%" PRIu32 "\n", matrix[y * g_width + column]);
    }
}

/**
 * Displays the value stored at the given element index
 */
void display_element(const uint32_t* matrix, ssize_t row, ssize_t column) {

    printf("%" PRIu32 "\n", matrix[row * g_width + column]);
}

////////////////////////////////
///   MATRIX INITALISATIONS  ///
////////////////////////////////

/**
 * Returns new matrix with all elements set to zero
 */
uint32_t* new_matrix(void) {

    return calloc(g_elements, sizeof(uint32_t));
}

/**
 * Returns new identity matrix
 */
uint32_t* identity_matrix(void) {

    uint32_t* matrix = new_matrix();

    for (ssize_t i = 0; i < g_width; i++) {
        matrix[i * g_width + i] = 1;
    }

    return matrix;
}

/**
 * Returns new matrix with elements generated at random using given seed
 */
uint32_t* random_matrix(uint32_t seed) {

    uint32_t* matrix = new_matrix();

    set_seed(seed);

    for (ssize_t i = 0; i < g_elements; i++) {
        matrix[i] = fast_rand();
    }

    return matrix;
}

/**
 * Returns new matrix with all elements set to given value
 */
uint32_t* uniform_matrix(uint32_t value) {

    uint32_t* matrix = new_matrix();

    for (ssize_t i = 0; i < g_elements; i++) {
        matrix[i] = value;
    }

    return matrix;
}

/**
 * Returns new matrix with elements in sequence from given start and step
 */
uint32_t* sequence_matrix(uint32_t start, uint32_t step) {

    uint32_t* matrix = new_matrix();
    uint32_t current = start;

    for (ssize_t i = 0; i < g_elements; i++) {
        matrix[i] = current;
        current += step;
    }

    return matrix;
}

////////////////////////////////
///     MATRIX OPERATIONS    ///
////////////////////////////////

/**
 * Returns new matrix with elements cloned from given matrix
 */
uint32_t* cloned(const uint32_t* matrix) {

    uint32_t* result = new_matrix();

    for (ssize_t i = 0; i < g_elements; i++) {
        result[i] = matrix[i];
    }

    return result;
}

/**
 * Returns new matrix with elements ordered in reverse
 */
uint32_t* reversed(const uint32_t* matrix) {

    uint32_t* result = new_matrix();

    for (ssize_t i = 0; i < g_elements; i++) {
        result[i] = matrix[g_elements - 1 - i];
    }

    return result;
}

/**
 * Returns new transposed matrix
 */
uint32_t* transposed(const uint32_t* matrix) {

    uint32_t* result = new_matrix();

    for (ssize_t y = 0; y < g_height; y++) {
        for (ssize_t x = 0; x < g_width; x++) {
            result[x * g_width + y] = matrix[y * g_width + x];
        }
    }

    return result;
}

/**
 * Returns new matrix with scalar added to each element
 */
uint32_t* scalar_add(const uint32_t* matrix, uint32_t scalar) {

    uint32_t* result = new_matrix();

    /*
        to do

        1 0        2 1
        0 1 + 1 => 1 2

        1 2        5 6
        3 4 + 4 => 7 8
    */
    for (ssize_t p = 0; p < g_elements; p++)
        result[p] = matrix[p] + scalar;

    return result;
}

/**
 * Returns new matrix with scalar multiplied to each element
 */
uint32_t* scalar_mul(const uint32_t* matrix, uint32_t scalar) {

    uint32_t* result = new_matrix();

    /*
        to do

        1 0        2 0
        0 1 x 2 => 0 2

        1 2        2 4
        3 4 x 2 => 6 8
    */

    for (ssize_t p = 0; p < g_elements; p++)
        result[p] = matrix[p] * scalar;

    return result;
}

/**
 * Returns new matrix with elements added at the same index
 */
uint32_t* matrix_add(const uint32_t* matrix_a, const uint32_t* matrix_b) {

    uint32_t* result = new_matrix();

    /*
        to do

        1 0   0 1    1 1
        0 1 + 1 0 => 1 1

        1 2   4 4    5 6
        3 4 + 4 4 => 7 8
    */

    for (ssize_t p = 0; p < g_elements; p++) {
        result[p] = matrix_a[p] + matrix_b[p];
    }

    return result;
}

/**
 * Returns new matrix, multiplying the two matrices together
 */
uint32_t* matrix_mul(const uint32_t* matrix_a, const uint32_t* matrix_b) {

    uint32_t* result = new_matrix();

    /*
        to do

        1 2   1 0    1 2
        3 4 x 0 1 => 3 4

        1 2   5 6    19 22
        3 4 x 7 8 => 43 50
    */

    uint32_t* bColJ = (uint32_t *) calloc(g_width, sizeof(uint32_t));
    ssize_t i, j, k;
    ssize_t s;
    const uint32_t *aRowI;

    for (j = 0; j < g_width; j++) {
        for (k = 0; k < g_width; k++)
            bColJ[k] = matrix_b[k*g_width + j];

        for (i = 0; i < g_height; i++) {
            aRowI = &matrix_a[i*g_width];
            s = 0;
            for (k = 0; k < g_width; k++)
                s += aRowI[k]*bColJ[k];

            result[i*g_width+j] = s;
        }
    }

    free(bColJ);

    return result;
}

/**
 * Returns new matrix, powering the matrix to the exponent
 */
uint32_t* matrix_pow(const uint32_t* matrix, uint32_t exponent) {

    uint32_t* result = new_matrix();

    /*
        to do

        1 2        1 0
        3 4 ^ 0 => 0 1

        1 2        1 2
        3 4 ^ 1 => 3 4

        1 2        199 290
        3 4 ^ 4 => 435 634
    */
    uint32_t* x = new_matrix();
    uint32_t* temp;

    ssize_t i, pX = 0, pY = 0;

    /* unify */
    for (i = 0; i < g_width; i++) {
        pX = pY + i;
        result[pX] = 1;
        pY += g_width;
    }

    /* copy matrix to x */
    for (i = 0; i < g_elements; i++)
        x[i] = matrix[i];

    /* exponent operation */
    while (exponent) {
        if (exponent & 1) {
           temp = matrix_mul(result, x);
           for (i = 0; i < g_elements; i++)
               result[i] = temp[i];
           free(temp);
        }

        temp = matrix_mul(x, x);
        for (i = 0; i < g_elements; i++) x[i] = temp[i];
        free(temp);

        exponent >>= 1;
    }

    free(x);

    return result;
}

////////////////////////////////
///       COMPUTATIONS       ///
////////////////////////////////

/**
 * Returns the sum of all elements
 */
uint32_t get_sum(const uint32_t* matrix) {

    /*
        to do

        1 2
        2 1 => 6

        1 1
        1 1 => 4
    */
    uint32_t total = 0;

    for (ssize_t p = 0; p < g_elements; p++)
        total += matrix[p];

    return total;
}

/**
 * Returns the trace of the matrix
 */
uint32_t get_trace(const uint32_t* matrix) {

    /*
        to do

        1 0
        0 1 => 2

        2 1
        1 2 => 4
    */
    uint32_t trace = 0;
    for (ssize_t p = 0; p < g_height; p++) {
        trace += matrix[p*g_width+p];
    }

    return trace;
}

/**
 * Returns the smallest value in the matrix
 */
uint32_t get_minimum(const uint32_t* matrix) {

    /*
        to do

        1 2
        3 4 => 1

        4 3
        2 1 => 1
    */
    uint32_t min = 0x7fffffff;

    for (ssize_t p = 0; p < g_elements; p++) {
        if (matrix[p] < min)
            min = matrix[p];
    }

    return min;
}

/**
 * Returns the largest value in the matrix
 */
uint32_t get_maximum(const uint32_t* matrix) {

    /*
        to do

        1 2
        3 4 => 4

        4 3
        2 1 => 4
    */
    uint32_t max = 0;

    for (ssize_t p = 0; p < g_elements; p++) {
        if (max < matrix[p])
            max = matrix[p];
    }

    return max;
}

/**
 * Returns the frequency of the value in the matrix
 */
uint32_t get_frequency(const uint32_t* matrix, uint32_t value) {

    /*
        to do

        1 1
        1 1 :: 1 => 4

        1 0
        0 1 :: 2 => 0
    */
    uint32_t freq = 0;

    for (ssize_t p = 0; p < g_elements; p++) {
        if (matrix[p] == value) freq++;
    }

    return freq;
}
