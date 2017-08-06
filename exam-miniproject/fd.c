#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "plotfunc.h"
#include "linalg.h"
#include "hamiltonian.h"
#include "powermethod.h"

#define pi 3.1415926535897932384626433832795028L

int main(int argc, char** argv) {
  if(argc != 6) {
    printf("Please give 5 arguments:\n");
    printf("   mass\n");
    printf("   omega\n");
    printf("   N (number of grid points)\n");
    printf("   M (number of power-method iterations)\n");
    printf("   nThreads (number of threads to use)\n");
    return -1;
  }
  double mass  = atof(argv[1]);
  double omega = atof(argv[2]);
  int N        = atoi(argv[3]);
  int M        = atoi(argv[4]);
  int nThreads = atoi(argv[5]);
  printf("mass     = %f\n", mass);
  printf("omega    = %f\n", omega);
  printf("N        = %d\n", N);
  printf("M        = %d\n", M);
  printf("nThreads = %d\n", nThreads);

  double x_max = 1.0;
  double h = x_max / N; /* h is the spacing between grid points */
  printf("h = %f\n", h);

  /* Timers */
  clock_t start, end;

  /* Construct Hamiltonian matrix H */
  double* H = (double*)malloc(N*N*sizeof(double));
  int i, j;
  start = clock();
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
      H[i*N+j] = get_H_matrix_element(i, j, N, h, mass, omega);
  end = clock();
  printf("---get_H_matrix_element time: %lf---\n", (double)(end-start)/CLOCKS_PER_SEC);

  start = clock();
  double one_norm_of_H = get_one_norm(N, H);
  end = clock();
  printf("---get_one_norm time: %lf---\n", (double)(end-start)/CLOCKS_PER_SEC);

  printf("one_norm_of_H = %f\n", one_norm_of_H);

  /* Create H_shifted as H shifted by one_norm_of_H in order to make
     the smallest eigenvalue the one with largest magnitude. */
  double* H_shifted = (double*)malloc(N*N*sizeof(double));
  for(i = 0; i < N; i++) {
    for(j = 0; j < N; j++) {
      H_shifted[i*N+j] = H[i*N+j];
      if(i == j)
	H_shifted[i*N+j] -= one_norm_of_H;
    }
  }

  /* Set up guess vector */
  double v_guess[N];
  for(i = 0; i < N; i++) {
    if(i <= N/2)
      v_guess[i] = i*0.001;
    else
      v_guess[i] = (N-i)*0.001;
  }
  double eigValApprox = 0;
  double v_from_power_method[N];
  int noOfIterations = M;

  start = clock();
  if (nThreads == 1) {
    do_power_method(N,
	  	  v_guess,
		    H_shifted,
		    &eigValApprox,
		    v_from_power_method,
		    noOfIterations);
  }
  else {
    parallel_pm(N,
                v_guess,
                H_shifted,
                &eigValApprox,
                v_from_power_method,
                noOfIterations,
                nThreads);
  }
  end = clock();
  printf("---do_power_method time: %lf---\n", (double)(end-start)/CLOCKS_PER_SEC);

  double eigValApprox_shiftedBack = eigValApprox + one_norm_of_H;
  printf("eigValApprox_shiftedBack = %f\n", eigValApprox_shiftedBack);

  /* Get wavefunction squared */
  double wavefunction_squared[N];
  for(i = 0; i < N; i++)
    wavefunction_squared[i] = v_from_power_method[i]*v_from_power_method[i];

  /* Get analytical solution */
  double wavefunction_analytical_squared[N];
  double x, psi;
  double massOmega = mass*omega;
  for(i = 0; i < N; i++) {
    double x = h*i - 0.5;
    double psi = pow(massOmega/pi, 0.25) * exp(-massOmega*x*x/2);
    wavefunction_analytical_squared[i] = psi*psi;
  }
  double energy_analytical = omega / 2;
  printf("energy_analytical = %f\n", energy_analytical);

  /* Plot potential */
  double potential_vec[N];
  start = clock();
  for(i = 0; i < N; i++)
    potential_vec[i] = get_potential_value(h*i, mass, omega);
  end = clock();
  printf("---get_potential_value time: %lf---\n", (double)(end-start)/CLOCKS_PER_SEC);/*
  plot_function(potential_vec, N, "potential");
  plot_function(wavefunction_squared, N, "wavefunction_squared");
  plot_function(wavefunction_analytical_squared, N, "wavefunction_analytical_squared");
*/
  /* Get wavefunction_squared_sum */
  double wavefunction_squared_sum = 0;
  for(i = 0; i < N; i++)
    wavefunction_squared_sum += wavefunction_squared[i];
  printf("wavefunction_squared_sum = %f\n", wavefunction_squared_sum);

  /* Normalize wavefunction */
  double wavefunction_normalized[N];
  double sqrt_h = sqrt(h);
  for(i = 0; i < N; i++)
    wavefunction_normalized[i] = v_from_power_method[i] / sqrt_h;
  double wavefunction_normalized_squared[N];
  for(i = 0; i < N; i++)
    wavefunction_normalized_squared[i] = wavefunction_normalized[i]*wavefunction_normalized[i];

  /* Get wavefunction_squared_max */
  double wavefunction_squared_max = 0;
  for(i = 0; i < N; i++) {
    if(wavefunction_normalized_squared[i] > wavefunction_squared_max)
      wavefunction_squared_max = wavefunction_normalized_squared[i];
  }
  printf("wavefunction_squared_max = %f\n", wavefunction_squared_max);
  double maxAbsDiff = 0;
  for(i = 0; i < N; i++) {
    double absDiff = fabs(wavefunction_normalized_squared[i] - wavefunction_analytical_squared[i]);
    if(absDiff > maxAbsDiff)
      maxAbsDiff = absDiff;
  }
  double maxRelDiff = maxAbsDiff / wavefunction_squared_max;
//  printf("Max rel difference between finite-difference computed and analytical wavefunction_squared value: %g\n", maxRelDiff);
  printf("Max rel difference: %g\n", maxRelDiff);

  double energy_diff = eigValApprox_shiftedBack - energy_analytical;
  printf("energy_diff = %13.9f\n", energy_diff);

  free(H);
  free(H_shifted);

  return 0;
}
