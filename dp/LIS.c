#include <stdio.h>

#define MAXN 1000

int a[MAXN];
int dp[MAXN];
int parent[MAXN];

int main() {
    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }

    int best_len = 0;
    int best_end = 0;

    for (int i = 0; i < n; i++) {
        dp[i] = 1;        // LIS kết thúc tại i ít nhất là chính nó
        parent[i] = -1;   // chưa có phần tử trước

        for (int j = 0; j < i; j++) {
            if (a[j] < a[i] && dp[j] + 1 > dp[i]) {
                dp[i] = dp[j] + 1;
                parent[i] = j;
            }
        }

        if (dp[i] > best_len) {
            best_len = dp[i];
            best_end = i;
        }
    }

    // In độ dài LIS
    printf("LIS length: %d\n", best_len);

    // Truy vết ngược để lấy dãy LIS
    int lis[MAXN];
    int idx = best_len - 1;
    int cur = best_end;

    while (cur != -1) {
        lis[idx--] = a[cur];
        cur = parent[cur];
    }

    // In dãy LIS
    printf("LIS sequence: ");
    for (int i = 0; i < best_len; i++) {
        printf("%d ", lis[i]);
    }
    printf("\n");

    return 0;
}

