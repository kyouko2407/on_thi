#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

long long n;
int nthreads;
long long *partial_sum;

void range(long tid, long long *start, long long *end) {
    long long chunk = n / nthreads;       // số phần tử "chuẩn" mỗi thread
    long long rem   = n % nthreads;       // số phần tử dư

    *start = tid * chunk + (tid < rem ? tid : rem) + 1; // +1 vì dãy bắt đầu từ 1
    *end   = *start + chunk + (tid < rem ? 1 : 0);      // end là >, duyệt [start, end)
}

void *psum_func(void *arg) {
    long tid = *(long *)arg;
    long long start, end;
    range(tid, &start, &end);

    long long s = 0;
    for (long long i = start; i < end; i++) {
        s += i;
    }
    partial_sum[tid] = s;
    return NULL;
}

int main() {
    scanf("%lld %d", &n, &nthreads);

    if (nthreads < 1) nthreads = 1;
    if (nthreads > n) nthreads = (int)n;  // không tạo nhiều thread hơn số phần tử

    partial_sum = (long long *)malloc(nthreads * sizeof(long long));
    pthread_t *tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    long *id = (long *)malloc(nthreads * sizeof(long));

    for (long i = 0; i < nthreads; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, psum_func, &id[i]);
    }

    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    long long total = 0;
    for (int i = 0; i < nthreads; i++) {
        total += partial_sum[i];
    }

    printf("Sum 1..%lld = %lld\n", n, total);

    free(partial_sum);
    free(tid);
    free(id);

    return 0;
}

