#include <stdio.h>
#include <math.h>
#include <pthread.h>

#include "powermethod.h"
#include "linalg.h"

void *parallel_pm(int N,
                  double *guess,
                  double *matrix,
                  double *resultEigVal,
                  double *resultVec,
                  int noOfIterations,
                  int nthreads) {

  /* Declarations */
  double v[N];
  int i, k;
  for(i = 0; i < N; i++)
    v[i] = guess[i];
  double eigValApprox = 0;
  double sqrt_vv_prod = 0;
  double y[N];
  double vv_prod, dotprod;

  /* Set up thread data */
  thread_data_t thread_data[nthreads];
  for (i=0; i<nthreads; i++) {
    thread_data[i].tid = i;
    thread_data[i].nthreads = nthreads;
    thread_data[i].n = N;
  }
  printf("sse:\n");
  for(k = 0; k < noOfIterations; k++) {

    pthread_t threads[nthreads];
    vv_prod = 0;
    dotprod = 0;
    for (i=0; i<nthreads; i++) {
      /* More thread data */
      thread_data[i].matrix = matrix;
      thread_data[i].x = v;
      thread_data[i].y = y;
      thread_data[i].scalar1 = &vv_prod;
      thread_data[i].scalar2 = &dotprod;
/*
      pthread_create(&threads[i],
                     NULL,
                     parallel_custom_mat_vec_mul,
                     (void *)&thread_data[i]);
*/
    }

    for (i=0; i<nthreads; i++) {
      //pthread_join(threads[i], NULL);
    }

//    do_mat_vec_mul(N, matrix, v, y);
    sse_mat_vec_mul(N, matrix, v, y);
    dotprod = get_dot_prod(N, v, y);
    vv_prod = get_dot_prod(N, v, v);

    /* Compute eigenvalue and update v */
    eigValApprox = dotprod / vv_prod;
    sqrt_vv_prod = sqrt(vv_prod);
    for(i = 0; i < N; i++)
      v[i] = y[i] / sqrt_vv_prod;
  }
  double norm_of_v = sqrt(get_dot_prod(N, v, v));
  for(i = 0; i < N; i++)
    resultVec[i] = v[i] / norm_of_v;
  *resultEigVal = eigValApprox;
}

void do_power_method(int N,
		     double* guess,
		     double* matrix,
		     double* resultEigVal,
		     double* resultVec,
		     int noOfIterations) {
  //printf("This is the do_power_method() function, N = %d, noOfIterations = %d\n", N, noOfIterations);
  double v[N];
  int i, k;
  for(i = 0; i < N; i++)
    v[i] = guess[i];
  double eigValApprox = 0;
  double sqrt_vv_prod = 0;
  for(k = 0; k < noOfIterations; k++) {
    double y[N];
    do_mat_vec_mul(N, matrix, v, y);
    double dotprod = get_dot_prod(N, v, y);
    double vv_prod = get_dot_prod(N, v, v);
    eigValApprox = dotprod / vv_prod;
    /*
    if(k % 2000 == 0)
      printf("k = %6d  eigValApprox = %17.10f\n", k, eigValApprox);
    */
    sqrt_vv_prod = sqrt(vv_prod);
    for(i = 0; i < N; i++)
      v[i] = y[i] / sqrt_vv_prod;
  }
  double norm_of_v = sqrt(get_dot_prod(N, v, v));
  for(i = 0; i < N; i++)
    resultVec[i] = v[i] / norm_of_v;
  *resultEigVal = eigValApprox;
}
