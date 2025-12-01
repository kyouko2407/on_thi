#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int M, K, N;
int nthreads;
double *A, *B, *C;

// Chia dải hàng [0..M-1] cho các thread
void range(long tid, int *start, int *end) {
    int chunk = M / nthreads;      // số hàng "chuẩn" mỗi thread
    int rem   = M % nthreads;      // số hàng dư
    *start = tid * chunk + (tid < rem ? tid : rem);
    *end   = *start + chunk + (tid < rem ? 1 : 0);
}

void *matrix_multiply_worker(void *arg) {
    long tid = *(long *)arg;
    int i_start, i_end;
    range(tid, &i_start, &i_end);

    for (int i = i_start; i < i_end; i++) {   // mỗi thread xử lý một số hàng của C
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int k = 0; k < K; k++) {
                sum += A[i * K + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
    return NULL;
}

int main() {
    scanf("%d %d %d %d", &M, &K, &N, &nthreads);

    if (nthreads < 1) nthreads = 1;
    if (nthreads > M) nthreads = M;   // không tạo nhiều thread hơn số hàng

    A = (double *)malloc(M * K * sizeof(double));
    B = (double *)malloc(K * N * sizeof(double));
    C = (double *)malloc(M * N * sizeof(double));

    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Nhập A (M x K)
    for (int i = 0; i < M; i++)
        for (int j = 0; j < K; j++)
            scanf("%lf", &A[i * K + j]);

    // Nhập B (K x N)
    for (int i = 0; i < K; i++)
        for (int j = 0; j < N; j++)
            scanf("%lf", &B[i * N + j]);

    pthread_t *tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    long *id = (long *)malloc(nthreads * sizeof(long));

    // Tạo thread
    for (long i = 0; i < nthreads; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, matrix_multiply_worker, &id[i]);
    }

    // Chờ thread xong
    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    // In kết quả
    printf("Matrix C = A * B:\n");
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2lf ", C[i * N + j]);
        }
        printf("\n");
    }

    free(A);
    free(B);
    free(C);
    free(tid);
    free(id);

    return 0;
}

