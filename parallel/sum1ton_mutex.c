#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long n;
int nthreads;
long long total_sum = 0;                 // biến tổng dùng chung
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void range(long tid, long long *start, long long *end) {
    long long chunk = n / nthreads;      // số phần tử "chuẩn" mỗi thread
    long long rem   = n % nthreads;      // số phần tử dư

    // chia [1..n] cho các thread, mỗi thread nhận một đoạn [start, end)
    *start = tid * chunk + (tid < rem ? tid : rem) + 1; // +1 vì bắt đầu từ 1
    *end   = *start + chunk + (tid < rem ? 1 : 0);
}

void *psum_func(void *arg) {
    long tid = *(long *)arg;
    long long start, end;
    range(tid, &start, &end);

    long long local_sum = 0;
    for (long long i = start; i < end; i++) {
        local_sum += i;
    }

    // Gộp vào tổng chung cần bảo vệ bằng mutex
    pthread_mutex_lock(&mtx);
    total_sum += local_sum;
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main() {
    scanf("%lld %d", &n, &nthreads);

    if (nthreads < 1) nthreads = 1;
    if (nthreads > n) nthreads = (int)n;   // không tạo nhiều thread hơn số phần tử

    pthread_t *tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    long *id = (long *)malloc(nthreads * sizeof(long));

    for (long i = 0; i < nthreads; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, psum_func, &id[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Sum 1..%lld = %lld\n", n, total_sum);

    free(tid);
    free(id);

    pthread_mutex_destroy(&mtx);
    return 0;
}

