#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

long long N;                // giới hạn trên
int nthreads;               // số luồng
long long global_sum = 0;   // tổng các số nguyên tố
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

// Hàm kiểm tra số nguyên tố (đủ nhanh cho N ~ 20 triệu)
int is_prime(long long x) {
    if (x < 2) return 0;
    if (x == 2) return 1;
    if (x % 2 == 0) return 0;
    long long r = (long long)sqrt((double)x);
    for (long long i = 3; i <= r; i += 2) {
        if (x % i == 0) return 0;
    }
    return 1;
}

// Chia đoạn [1..N] cho các thread
// Mỗi thread tid nhận [start, end] (INCLUSIVE)
void range(long tid, long long *start, long long *end) {
    long long chunk = N / nthreads;      // số phần tử "chuẩn" mỗi thread
    long long rem   = N % nthreads;      // số phần tử dư

    *start = tid * chunk + (tid < rem ? tid : rem) + 1; // +1 vì bắt đầu từ 1
    *end   = *start + chunk + (tid < rem ? 1 : 0) - 1;  // inclusive

    if (*start > N) *start = N + 1;     // đoạn rỗng
    if (*end   > N) *end   = N;
}

void *prime_sum_worker(void *arg) {
    long tid = *(long *)arg;
    long long start, end;
    range(tid, &start, &end);

    long long local_sum = 0;

    for (long long x = start; x <= end; x++) {
        if (is_prime(x)) {
            local_sum += x;
        }
    }

    // Cộng vào tổng chung – cần mutex
    pthread_mutex_lock(&mtx);
    global_sum += local_sum;
    pthread_mutex_unlock(&mtx);

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nthreads>\n", argv[0]);
        return 1;
    }

    nthreads = (int)strtol(argv[1], NULL, 10);
    if (nthreads < 1) nthreads = 1;

    // Nhập N (ví dụ: 20000000)
    if (scanf("%lld", &N) != 1) {
        fprintf(stderr, "Failed to read N\n");
        return 1;
    }

    if (N < 1) {
        printf("Sum of primes in [1..%lld] = 0\n", N);
        return 0;
    }

    if (nthreads > N) nthreads = (int)N; // không tạo nhiều thread hơn số phần tử

    pthread_t *tid = (pthread_t *)malloc(nthreads * sizeof(pthread_t));
    long *id = (long *)malloc(nthreads * sizeof(long));

    // Tạo M luồng
    for (long i = 0; i < nthreads; i++) {
        id[i] = i;
        pthread_create(&tid[i], NULL, prime_sum_worker, &id[i]);
    }

    // Chờ các luồng hoàn thành
    for (int i = 0; i < nthreads; i++) {
        pthread_join(tid[i], NULL);
    }

    printf("Sum of primes in [1..%lld] = %lld\n", N, global_sum);

    free(tid);
    free(id);
    pthread_mutex_destroy(&mtx);
    return 0;
}

