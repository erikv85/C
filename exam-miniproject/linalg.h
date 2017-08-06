/**
 * @file linalg.h
 *
 * @author Erik Vesterlund unless stated otherwise.
 *
 * @brief ...
 */

#ifndef LINALG_H
#define LINALG_H

/**
 * @brief Struct containing data for a thread to work on.
 */
typedef struct thread_data_s {
  int tid;         /**< thread's ID */
  int nthreads;    /**< number of threads */
  int n;           /**< size of the data */
  double *matrix;  /**< an n-by-n matrix */
  double *x;       /**< an n-by-1 vector */
  double *y;       /**< an n-by-1 vector */
  double *scalar1; /**< a general-purpose scalar */
  double *scalar2; /**< a general-purpose scalar */
} thread_data_t;

/**
 * @brief Customized parallel matrix-vector multiplication.
 *
 * A function doing matrix-vector multiplication utilizing the
 * structure of the matrices used in the related project.
 *
 * @param arg Void pointer to thread data
 * @return NULL
 */
void *parallel_custom_mat_vec_mul(void *arg);

/**
 * @brief Parallel matrix-vector multiplication.
 *
 * @param arg Void pointer to thread data
 * @return NULL
 */
void *parallel_mat_vec_mul(void *arg);

/**
 * @brief Matrix-vector multiplication using SIMD instructions.
 *
 * @param n Size of data
 * @param matrix Pointer to n-by-n matrix
 * @param x Pointer to n-by-1 vector
 * @param y Pointer to n-by-1 vector
 */
void sse_mat_vec_mul(int n, double *matrix, double *x, double *y);

/**
 * @brief Naive matrix-vector multiplication.
 *
 * @param n Size of data
 * @param matrix Pointer to n-by-n matrix
 * @param x Pointer to an n-by-1 vector
 * @param y Pointer to an n-by-1 vector
 */
void do_mat_vec_mul(int n, const double* matrix, const double* x, double* y);

/**
 * @brief Calculate the dot product of two vectors.
 *
 * @author Elias Rudberg (?)
 *
 * @param N Vector length
 * @param a Pointer to an N-by-1 vector
 * @param b Pointer to an N-by-1 vector
 * @return The dot product of a and b
 */
double get_dot_prod(int N, const double* a, const double* b);

/**
 * @brief Calculate the one-norm of a matrix.
 *
 * @author Elias Rudberg (?), edited by Erik Vesterlund.
 *
 * @param N Matrix dimension
 * @param A Pointer to an N-by-N matrix.
 * @return The one-norm of A
 */
double get_one_norm(int N, const double* A);

#endif /* LINALG_H */
