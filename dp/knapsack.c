#include <stdio.h>

#define MAXN 1000
#define MAXW 1000
#define max(a,b) ((a) > (b) ? (a) : (b))

int w[MAXN + 1];
int v[MAXN + 1];
int dp[MAXN + 1][MAXW + 1];

int main() {
    int n, W;
    scanf("%d %d", &n, &W);

    for (int i = 1; i <= n; i++) {
        scanf("%d", &w[i]);
    }
    for (int i = 1; i <= n; i++) {
        scanf("%d", &v[i]);
    }

    // DP tính giá trị lớn nhất
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= W; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else {
                dp[i][j] = dp[i-1][j]; // không lấy đồ i
                if (j >= w[i]) {
                    int take = v[i] + dp[i-1][j - w[i]];
                    if (take > dp[i][j]) dp[i][j] = take;
                }
            }
        }
    }

    printf("Max value: %d\n", dp[n][W]);

    // Truy vết xem những đồ nào được chọn
    int res = dp[n][W];
    int j = W;
    int chosen[MAXN];
    int cnt = 0;

    for (int i = n; i >= 1; i--) {
        if (res <= 0) break;

        if (res == dp[i-1][j]) {
            // không chọn đồ i
            continue;
        } else {
            // chọn đồ i
            chosen[cnt++] = i;
            res -= v[i];
            j   -= w[i];
        }
    }

    // In các đồ được chọn (theo thứ tự tăng dần chỉ số)
    printf("Chosen items (index): ");
    for (int i = cnt - 1; i >= 0; i--) {
        printf("%d ", chosen[i]);
    }
    printf("\n");

    return 0;
}

