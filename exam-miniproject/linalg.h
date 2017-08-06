typedef struct thread_data_s {
  int tid;
  int nthreads;
  int n;
  double *matrix;
  double *x;
  double *y;
  double *scalar1;
  double *scalar2;
} thread_data_t;

void *parallel_custom_mat_vec_mul(void *arg);

void *parallel_mat_vec_mul(void *arg);

void sse_mat_vec_mul(int n, double *matrix, double *x, double *y);

void do_mat_vec_mul(int n, const double* matrix, const double* x, double* y);

double get_dot_prod(int N, const double* a, const double* b);

double get_one_norm(int N, const double* A);
