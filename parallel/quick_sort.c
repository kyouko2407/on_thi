#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAXN 1000000

int *a;                 // mảng cần sắp xếp
int n;                  // số phần tử
int max_depth;          // độ sâu tối đa để tạo thread (2^max_depth <= nthreads)

// ================== QUICK SORT TUẦN TỰ ==================
void quicksort_seq(int l, int r) {
    if (l >= r) return;
    int i = l, j = r;
    int pivot = a[(l + r) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            int tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }
    }
    if (l < j) quicksort_seq(l, j);
    if (i < r) quicksort_seq(i, r);
}

// ================== QUICK SORT SONG SONG ==================

typedef struct {
    int l, r;
    int depth;
} QSArgs;

void parallel_quicksort(int l, int r, int depth);

void *qs_thread(void *arg) {
    QSArgs *args = (QSArgs *)arg;
    parallel_quicksort(args->l, args->r, args->depth);
    free(args);  // giải phóng struct đã cấp phát
    return NULL;
}

void parallel_quicksort(int l, int r, int depth) {
    if (l >= r) return;

    // Nếu đã đạt độ sâu tối đa -> sort tuần tự để không tạo thêm thread
    if (depth >= max_depth) {
        quicksort_seq(l, r);
        return;
    }

    int i = l, j = r;
    int pivot = a[(l + r) / 2];

    while (i <= j) {
        while (a[i] < pivot) i++;
        while (a[j] > pivot) j--;
        if (i <= j) {
            int tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            i++;
            j--;
        }
    }

    pthread_t tid_left, tid_right;
    int use_left = 0, use_right = 0;

    // Nhánh trái [l..j]
    if (l < j) {
        QSArgs *args = (QSArgs *)malloc(sizeof(QSArgs));
        args->l = l;
        args->r = j;
        args->depth = depth + 1;
        pthread_create(&tid_left, NULL, qs_thread, args);
        use_left = 1;
    }

    // Nhánh phải [i..r]
    if (i < r) {
        QSArgs *args = (QSArgs *)malloc(sizeof(QSArgs));
        args->l = i;
        args->r = r;
        args->depth = depth + 1;
        pthread_create(&tid_right, NULL, qs_thread, args);
        use_right = 1;
    }

    if (use_left)  pthread_join(tid_left, NULL);
    if (use_right) pthread_join(tid_right, NULL);
}

// ================== MAIN ==================

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <nthreads>\n", argv[0]);
        return 1;
    }

    int nthreads = (int)strtol(argv[1], NULL, 10);
    if (nthreads < 1) nthreads = 1;

    if (scanf("%d", &n) != 1) {
        fprintf(stderr, "Failed to read n\n");
        return 1;
    }

    a = (int *)malloc(n * sizeof(int));
    if (!a) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    // Tính max_depth sao cho 2^max_depth <= nthreads
    max_depth = 0;
    while ((1 << (max_depth + 1)) <= nthreads) {
        max_depth++;
    }
    // Ví dụ:
    // nthreads = 1 -> max_depth = 0 (chạy hoàn toàn tuần tự)
    // nthreads = 2 -> max_depth = 1 (tối đa 2 thread)
    // nthreads = 4..7 -> max_depth = 2 (tối đa ~4 thread)
    // nthreads = 8..15 -> max_depth = 3, ...

    parallel_quicksort(0, n - 1, 0);

    // In kết quả
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    return 0;
}

