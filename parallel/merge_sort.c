#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *a;        // mảng cần sắp xếp
int *tmp;      // mảng tạm để merge
int n;
int max_depth; // giới hạn độ sâu song song

// ================== MERGE & MERGESORT TUẦN TỰ ==================

void merge(int l, int mid, int r) {
    int i = l, j = mid + 1, k = l;

    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) tmp[k++] = a[i++];
        else               tmp[k++] = a[j++];
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r)   tmp[k++] = a[j++];

    for (int t = l; t <= r; t++) {
        a[t] = tmp[t];
    }
}

void mergesort_seq(int l, int r) {
    if (l >= r) return;
    int mid = (l + r) / 2;
    mergesort_seq(l, mid);
    mergesort_seq(mid + 1, r);
    merge(l, mid, r);
}

// ================== MERGESORT SONG SONG ==================

typedef struct {
    int l, r;
    int depth;
} MSArgs;

void parallel_mergesort(int l, int r, int depth);

void *ms_thread(void *arg) {
    MSArgs *args = (MSArgs *)arg;
    parallel_mergesort(args->l, args->r, args->depth);
    free(args);
    return NULL;
}

void parallel_mergesort(int l, int r, int depth) {
    if (l >= r) return;

    // Hết quota song song → sort tuần tự
    if (depth >= max_depth) {
        mergesort_seq(l, r);
        return;
    }

    int mid = (l + r) / 2;

    pthread_t tid_left, tid_right;
    int use_left = 0, use_right = 0;

    // Nhánh trái [l..mid]
    MSArgs *argsL = (MSArgs *)malloc(sizeof(MSArgs));
    argsL->l = l;
    argsL->r = mid;
    argsL->depth = depth + 1;
    pthread_create(&tid_left, NULL, ms_thread, argsL);
    use_left = 1;

    // Nhánh phải [mid+1..r]
    MSArgs *argsR = (MSArgs *)malloc(sizeof(MSArgs));
    argsR->l = mid + 1;
    argsR->r = r;
    argsR->depth = depth + 1;
    pthread_create(&tid_right, NULL, ms_thread, argsR);
    use_right = 1;

    if (use_left)  pthread_join(tid_left, NULL);
    if (use_right) pthread_join(tid_right, NULL);

    // Hai nửa đã được sort → merge
    merge(l, mid, r);
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

    a   = (int *)malloc(n * sizeof(int));
    tmp = (int *)malloc(n * sizeof(int));
    if (!a || !tmp) {
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
    // nthreads=1  -> max_depth=0 (hoàn toàn tuần tự)
    // nthreads=2  -> max_depth=1
    // nthreads=3..4 -> max_depth=2
    // nthreads=5..8 -> max_depth=3, ...

    parallel_mergesort(0, n - 1, 0);

    // In kết quả
    for (int i = 0; i < n; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");

    free(a);
    free(tmp);
    return 0;
}

