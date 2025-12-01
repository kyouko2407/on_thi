// vector_add_dot.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int n, nthreads;
double *A, *B, *C;
double *partial_dot;

void range(long tid, int *start, int *end) {
    int chunk = n / nthreads;
    int rem   = n % nthreads;
    *start = tid * chunk + (tid < rem ? tid : rem);
    *end   = *start + chunk + (tid < rem ? 1 : 0);
}

void *vector_add_dot(void *arg) {
    long tid = *(long *)arg;
    int start, end;
    range(tid, &start, &end);

    double local_dot = 0.0;
    for (int i = start; i < end; i++) {
        C[i] = A[i] + B[i];
        local_dot += A[i] * B[i];
    }
    partial_dot[tid] = local_dot;
    return NULL;
}

int main() {
    scanf("%d %d", &n, &nthreads);

    A = (double *)malloc(n * sizeof(double));
    B = (double *)malloc(n * sizeof(double));
    C = (double *)malloc(n * sizeof(double));
    partial_dot = (double *)malloc(nthreads * sizeof(double));

    for (int i = 0; i < n; i++) scanf("%lf", &A[i]);
    for (int i = 0; i < n; i++) scanf("%lf", &B[i]);

    pthread_t *tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    long *id = (long *)malloc(nthreads * sizeof(long));

    for (long i = 0; i < nthreads; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, vector_add_dot, &id[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    double dot = 0.0;
    for (int i = 0; i < nthreads; i++) dot += partial_dot[i];

    printf("Vector C (A+B):\n");
    for (int i = 0; i < n; i++) {
        printf("%.2f ", C[i]);
    }
    printf("\n");

    printf("Dot product: %.2f\n", dot);

    free(A);
    free(B);
    free(C);
    free(partial_dot);
    free(tid);
    free(id);

    return 0;
}

