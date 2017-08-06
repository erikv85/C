void *parallel_pm(int N,
		     double* guess,
		     double* matrix,
		     double* resultEigVal,
		     double* resultVec,
		     int noOfIterations,
		     int nthreads);

void do_power_method(int N,
		     double* guess,
		     double* matrix,
		     double* resultEigVal,
		     double* resultVec,
		     int noOfIterations);
