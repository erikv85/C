#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <smmintrin.h>  /* SSE4 types and funcs */
#include <string.h>     /* memset in sse_mat_vec_mul */

#include "linalg.h"

pthread_mutex_t sum_mutex;

void *parallel_custom_mat_vec_mul(void *arg) {
  thread_data_t mydata = *(thread_data_t *)arg;
  int tid = mydata.tid;
  int nthreads = mydata.nthreads;
  int n = mydata.n;
  double *matrix = mydata.matrix;
  double *x = mydata.x;
  double *y = mydata.y;
  double *vv_prod = mydata.scalar1;
  double *dotprod = mydata.scalar2;

  int lb = tid*(n/nthreads);
  int ub = (tid+1)*(n/nthreads);
  int rem = n%nthreads;
  if (tid == 0)
    lb++;
  else if (tid == (nthreads-1)) {
    ub--;
    /* If n isn't divisible by nthreads,
     * the "last" thread does the
     * remaining work. */
    if (rem != 0)
      ub = n-1;
  }

  double local_vv_prod = 0;
  double local_dotprod = 0;

  int i,j;
  double sum;
  if (tid == 0) {
    y[0] = matrix[0]*x[0] + matrix[1]*x[1] + matrix[n-1]*x[n-1];
    local_vv_prod += x[0]*x[0];
    local_dotprod += x[0]*y[0];
  }

  for (i=lb; i<ub; i++) {
    y[i] = matrix[i*n+(i-1)]*x[i-1] + matrix[i*n+i]*x[i] + matrix[i*n+(i+1)]*x[i+1];
    local_vv_prod += x[i]*x[i];
    local_dotprod += x[i]*y[i];
  }

  if (tid == (nthreads-1)) {
    y[n-1] = matrix[n*(n-1)]*x[0] + matrix[n*(n-1)+n-2]*x[n-2] + matrix[n*(n-1)+n-1]*x[n-1];
    local_vv_prod += x[n-1]*x[n-1];
    local_dotprod += x[n-1]*y[n-1];
  }

  pthread_mutex_lock(&sum_mutex);
  *vv_prod = (*vv_prod)+local_vv_prod;
  *dotprod = (*dotprod)+local_dotprod;
  pthread_mutex_unlock(&sum_mutex);
}

void *parallel_mat_vec_mul(void *arg) {

  thread_data_t mydata = *(thread_data_t *)arg;
  int tid = mydata.tid;
  int nthreads = mydata.nthreads;
  int n = mydata.n;
  double *matrix = mydata.matrix;
  double *x = mydata.x;
  double *y = mydata.y;
  double *vv_prod = mydata.scalar1;
  double *dotprod = mydata.scalar2;

  int lb = tid*(n/nthreads);
  int ub = (tid+1)*(n/nthreads);
  int rem = n%nthreads;
  if (tid == (nthreads-1) && (rem != 0)) {
    ub = n;
  }

  double local_vv_prod = 0;
  double local_dotprod = 0;

  int i,j;
  double sum;
  for (i=lb; i<ub; i++) {
    sum = 0;
    for (j=0; j<n; j++) {
      sum += matrix[i*n+j]*x[j];
    }
    y[i] = sum;
    local_vv_prod += x[i]*x[i];
    local_dotprod += x[i]*y[i];
  }

  pthread_mutex_lock(&sum_mutex);
  *vv_prod = (*vv_prod)+local_vv_prod;
  *dotprod = (*dotprod)+local_dotprod;
  pthread_mutex_unlock(&sum_mutex);
}

void sse_mat_vec_mul(int n, double *matrix, double *x, double *y) {
  int row, col, tmp;
  __m128d v, w, summand;
  memset(y, 0, n*sizeof(double));
  for (row=0; row<n; row++) {
    tmp = row*n;
    for (col=0; col<n; col+=2) {
      v = _mm_load_pd(matrix+tmp+col);
      w = _mm_load_pd(x+col);

      summand = _mm_dp_pd(v, w, 0xF1);
      y[row] += _mm_cvtsd_f64(summand);
    }
  }
}

void do_mat_vec_mul(int n, const double* matrix, const double* x, double* y) {
  int i, j;
  for(i = 0; i < n; i++) {
    double sum = 0;
    for(j = 0; j < n; j++)
      sum += matrix[i*n+j]*x[j];
    y[i] = sum;
  }
}

double get_dot_prod(int N, const double* a, const double* b) {
  double sum = 0;
  int i;
  for(i = 0; i < N; i++)
    sum += a[i] * b[i];
  return sum;
}

/* Note: since we assume the matrix is symmetric, the one-norm and the
   inf-norm are the same, so it does not matter if we take row sums or
   column sums. */
double get_one_norm(int N, const double* A) {
  double maxColSum = 0;
  int i, k;
  for(i = 0; i < N; i++) {
    double currColSum = 0;
    for(k = 0; k < N; k++)
      currColSum += fabs(A[i*N+k]);
    if(currColSum > maxColSum)
      maxColSum = currColSum;
  }
  return maxColSum;
}
