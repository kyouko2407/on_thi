#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_T 8
#define N 4

int nthreads;
double *A, *B, *C;

void range(long tid, int *start, int *end)
{
    int chunk = N / nthreads;
    int rem   = N % nthreads;
    *start = tid * chunk + (tid < rem ? tid : rem);
    *end   = *start + chunk + (tid < rem ? 1 : 0);
}

void *matrix1(void *arg)
{
    long myid = *((long *)arg);
    int i_start, i_end;
    range(myid, &i_start, &i_end);

    for (int i = i_start; i < i_end; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{

    nthreads = atoi(argv[1]);

    A = malloc(N * N * sizeof(double));
    B = malloc(N * N * sizeof(double));
    C = malloc(N * N * sizeof(double));
    if (!A || !B || !C) { perror("malloc"); return 1; }

    double A_init[N][N] = {
        {1, 2, 3, 4},
        {2, 3, 4, 5},
        {3, 4, 5, 6},
        {4, 5, 6, 7}
    };
    double B_init[N][N] = {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1}
    };
    for (int i = 0; i < N * N; i++) {
        A[i] = ((double *)A_init)[i];
        B[i] = ((double *)B_init)[i];
        C[i] = 0.0;
    }

    pthread_t tid[MAX_T];
    long myid[MAX_T];
    struct timespec t0, t1;

    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (long t = 0; t < nthreads; t++) {
        myid[t] = t;
        pthread_create(&tid[t], NULL, matrix1, &myid[t]);
    }
    for (int t = 0; t < nthreads; t++)
        pthread_join(tid[t], NULL);

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = (t1.tv_sec - t0.tv_sec) + (t1.tv_nsec - t0.tv_nsec) / 1e9;

    printf("Threads = %d, N = %d\n", nthreads, N);
    printf("Elapsed time = %.6f s\n", elapsed);

    printf("\nMatrix C = A * B:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            printf("%6.1f ", C[i * N + j]);
        printf("\n");
    }

    free(A); free(B); free(C);
    return 0;
}
